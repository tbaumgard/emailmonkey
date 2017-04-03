<?php

/**
 * @file
 * Miscellaneous functions.
 */

function emailmonkey_create_machine_name($human_name) {
  // https://api.drupal.org/api/drupal/developer%21topics%21forms_api_reference.html/7.x#machine_name
  return strtolower(preg_replace('/[^a-zA-Z0-9_]+/', '_', $human_name));
}

function emailmonkey_node_url($node, $url_type) {
  if ($url_type == 'node') {
    return 'node/' . $node->nid;
  }

  $okay_url_types = array('delete', 'export', 'preview', 'raw', 'send', 'test', 'update');
  $base = 'emailmonkey/' . $node->nid . '/';

  if (in_array($url_type, $okay_url_types)) {
    return $base . $url_type;
  }
  else {
    return $base . 'preview';
  }
}

/**
 * Call MailChimp API library object method.
 *
 * If this function is unable to get successfully call the method, for whatever
 * reason, a log entry will be created and a message will be displayed to the
 * user.
 *
 * @param string $type
 *   MailChimp API library object type, e.g., 'lists', 'campaigns', or
 *   'root'.
 * @param string $method
 *   The method to call on the API library object.
 * @param array $arguments
 *   Arguments to pass to the API library object method.
 * @param array $options
 *   An array containing one or more of the following options:
 *   - use_response_cache: when TRUE, which is the default, cache responses and
 *     return cached responses when available.
 *
 * @return
 *   Returns the appropriate MailChimp API library object or NULL on failure.
 *
 * @throws EmailMonkey_Exception
 *   When an unexpected error occurs.
 */
function emailmonkey_api($type, $method) {
  $cache = &drupal_static(__FUNCTION__, array());
  $arguments = array_slice(func_get_args(), 2);

  if (empty($cache[$type])) {
    emailmonkey_load_libraries();

    $api_key = variable_get('emailmonkey_api_key');

    if ($type == 'lists') {
      $cache['objects'][$type] = new Mailchimp\MailchimpLists($api_key);
    }
    elseif ($type == 'interests') {
      $cache['objects'][$type] = new Mailchimp\MailchimpLists($api_key);
    }
    elseif ($type == 'segments') {
      $cache['objects'][$type] = new Mailchimp\MailchimpLists($api_key);
    }
    elseif ($type == 'campaigns') {
      $cache['objects'][$type] = new Mailchimp\MailchimpCampaigns($api_key);
    }
    elseif ($type == 'root') {
      $cache['objects'][$type] = new Mailchimp\Mailchimp($api_key);
    }
    else {
      throw new EmailMonkey_Exception(t('Invalid MailChimp API library type used.'));
    }
  }

  try {
    return call_user_func_array(array($cache['objects'][$type], $method), $arguments);
  }
  catch (Mailchimp\MailchimpAPIException $e) {
    emailmonkey_exception_alert($e);

    throw new EmailMonkey_Exception(t('MailChimp API error'), $e);
  }
}

/**
 * Given an exception thrown by calling a method in the MailChimp API library,
 * create a log entry and display a message to the user.
 *
 * @param Exception $exception
 *   The exception thrown.
 */
function emailmonkey_exception_alert($exception) {
  $error = $exception->getMessage();
  $previous_exception = $exception->getPrevious();

  if ($previous_exception instanceof Mailchimp\MailchimpAPIException) {
    $error = $previous_exception->getMessage();
    $previous_exception = $previous_exception->getPrevious();
  }

  if ($previous_exception instanceof GuzzleHttp\Exception\RequestException) {
    $response = $previous_exception->getResponse();

    if (!empty($response)) {
      $body = $response->getBody();

      if (is_string($body) && strlen($body) > 0 && $body{0} == '{') {
        $json = json_decode($body);

        if ($json instanceof stdClass) {
          $error = l($json->detail, $json->type);
        }
      }
    }
  }

  $message = t('An EmailMonkey error occurred: @error.', array('@error' => $error));
  watchdog('emailmonkey', $message, NULL, WATCHDOG_ERROR);
}

/**
 * Change the relative URLs in the given HTML to absolute URLs. This function
 * uses the Pathologic module to perform the rewriting.
 *
 * @param string $html
 *   The HTML to modify.
 *
 * @see _pathologic_filter()
 * @see checkup_markup()
 */
function emailmonkey_rewrite_urls($html) {
  $loaded = &drupal_static(__FUNCTION__, FALSE);

  // Some of the logic and settings in _pathologic_filter() need to be
  // overwritten, namely that the protocol style should always default to full.
  // So, on the first run of this function, call it with some empty dummy text
  // to initialize the settings and then overwrite them.
  if (empty($loaded)) {
    $loaded = TRUE;
    emailmonkey_rewrite_urls('');
    $pathologic_settings = &drupal_static('_pathologic_filter', array());
    $pathologic_settings['emailmonkey_dummy_format']['protocol_style'] = 'full';
  }

  // Most of the settings/values below are unimportant and unused by
  // _pathologic_filter(), $filter->format being the big exception, but sane
  // defaults are used in case that changes at some point. It's unlikely that
  // the function signature will change given that Drupal 7 won't be making any
  // major API changes at this point, and _pathologic_filter() relies on
  // Drupal's filter API.

  $filter = new stdClass;
  $filter->format = 'emailmonkey_dummy_format';
  $filter->module = 'emailmonkey';
  $filter->name = 'emailmonkey_dummy_filter';
  $filter->weight = -50;
  $filter->status = TRUE;
  $filter->settings = array();

  $format = new stdClass;
  $format->format = 'emailmonkey_dummy_format';
  $format->name = t('EmailMonkey Dummy Format');
  $format->cache = TRUE;
  $format->status = TRUE;
  $format->weight = -50;

  $langcode = '';
  $cache = TRUE;
  $cache_id = $format->format . ':' . $langcode . ':' . hash('sha256', $html);

  // Preserve MailChimp merge tags inside what appear to be HTML attributes so
  // that Pathologic doesn't automatically append the base URL.
  $emailmonkey_attribute = 'data-emailmonkey-escape-';
  $attribute_regexp = '(\\S+\\s*=\\s*(["\'])\\s*\\*\\|.+?\\|\\*\\s*\\2)';
  $revert_regexp = $emailmonkey_attribute . $attribute_regexp;

  $html = preg_replace('/'.$attribute_regexp.'/', $emailmonkey_attribute.'$0', $html);
  $html = _pathologic_filter($html, $filter, $format, $langcode, $cache, $cache_id);
  $html = preg_replace('/'.$revert_regexp.'/', '$1', $html);

  return $html;
}

/**
 * Minify HTML.
 */
function emailmonkey_minify_html($html) {
  // There's no need to minify the CSS since it should be processed by an
  // inliner, which should remove the styles after inlining.
  return Minify_HTML::minify($html);
}

/**
 * Emogrify HTML to inline its CSS.
 */
function emailmonkey_emogrify_html($html) {
  $emogrifier = new Pelago\Emogrifier($html);
  $emogrifier->disableInlineStyleAttributesParsing();
  $emogrifier->enableCssToHtmlMapping();

  $html = $emogrifier->emogrify();

  // Because Emogrifier uses DOMElement, MailChimp merge tags in attributes will
  // be escaped to entity codes, so they need to be unescaped so that MailChimp
  // can interpret them.
  $attribute_regexp = '/(\\S+\\s*=\\s*)(["\'])(\\s*\\*)%7[cC](.+?)%7[cC](\\*\\s*\\2)/';
  $html = preg_replace($attribute_regexp, '$1$2$3|$4|$5', $html);

  return $html;
}

/**
 * Fully render a node as a full HTML page.
 */
function emailmonkey_render_raw_node_with_wrapper($node) {
  $html = theme('emailmonkey_raw', array('node' => $node));
  $html = emailmonkey_rewrite_urls($html);
  $html = emailmonkey_minify_html($html);
  $html = emailmonkey_emogrify_html($html);

  return $html;
}

/**
 * Render only a node for exporting and inserting into existing HTML.
 */
function emailmonkey_render_raw_node($node) {
  global $EMAILMONKEY_EXPORTING;
  $EMAILMONKEY_EXPORTING = TRUE;

  $node_data = emailmonkey_data_node_data_for_node($node);
  $node_view = node_view($node, $node_data['view_mode']);
  $rendered_node = drupal_render($node_view);

  $EMAILMONKEY_EXPORTING = FALSE;

  return $rendered_node;
}
