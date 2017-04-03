<?php

/**
 * @file
 * Functions used to handle forms.
 */

/**
 * Create a form for admin settings.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_admin_settings() {
  emailmonkey_load_includes();

  $form['emailmonkey_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('MailChimp API key'),
    '#description' => t('The API key for your MailChimp account. Get or generate an API key in the !apidashboard.', array('!apidashboard' => l(t('MailChimp API Dashboard'), 'https://admin.mailchimp.com/account/api'))),
    '#default_value' => variable_get('emailmonkey_api_key', ''),
    '#size' => 37,
    '#maxlength' => 45,
    '#element_validate' => array('emailmonkey_form_validate_api_key'));

  $form['emailmonkey_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types'),
    '#description' => t('The content types for which to enable MailChimp functionality.'),
    '#default_value' => variable_get('emailmonkey_content_types', array()),
    '#options' => node_type_get_names());

  if (!emailmonkey_are_libraries_installed()) {
    $form['emailmonkey_api_key']['#disabled'] = TRUE;
    unset($form['emailmonkey_api_key']['#element_validate']);
  }

  $form = system_settings_form($form);

  $form['#submit'][] = 'emailmonkey_form_admin_settings_submit';

  return $form;
}

/**
 * Handle forms submitted for admin settings.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_admin_settings_submit($form, &$form_state) {
  if (!emailmonkey_are_libraries_installed()) {
    return;
  }

  // Make sure the API key is set.
  variable_set('emailmonkey_api_key', $form_state['values']['emailmonkey_api_key']);

  try {
    $account = emailmonkey_accounts_get_account_basics();
  }
  catch (EmailMonkey_Exception $e) {
    drupal_set_message(t('Failed to save the MailChimp account information. Please resubmit the form.'));
    return;
  }

  variable_set('emailmonkey_account_id', $account->account_id);
}

/**
 * Create a form for configuring styles.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_configure_styles($form, &$form_state) {
  $styles = variable_get('emailmonkey_styles', array());
  $offset = 0;

  foreach ($styles as $machine_name => $style) {
    $form["emailmonkey_style_{$offset}"] = array(
      '#type' => 'fieldset',
      '#title' => t('Style #@number', array('@number' => $offset+1)),
      '#collapsible' => FALSE);

    $form["emailmonkey_style_{$offset}"]["emailmonkey_style_machine_name_{$offset}"] = array(
      '#type' => 'hidden',
      '#value' => $machine_name,
      '#element_validate' => array('emailmonkey_form_validate_machine_name'));

    $form["emailmonkey_style_{$offset}"]["emailmonkey_style_name_{$offset}"] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('A descriptive name for the style.'),
      '#default_value' => $style['name'],
      '#size' => 45,
      '#maxlength' => 45,
      '#element_validate' => array('emailmonkey_form_validate_existing_style_name'));

    $form["emailmonkey_style_{$offset}"]["emailmonkey_style_path_{$offset}"] = array(
      '#type' => 'textfield',
      '#title' => t('Style sheet path'),
      '#description' => t('Path to the style sheet relative to the Drupal root.'),
      '#default_value' => $style['path'],
      '#size' => 100,
      '#element_validate' => array('emailmonkey_form_validate_style_path'));

    $form["emailmonkey_style_{$offset}"]["emailmonkey_style_delete_{$offset}"] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete style'),
      '#description' => t('Deleting this style will also automatically remove it from any view modes.'));

    $offset += 1;
  }

  $form["emailmonkey_style_new"] = array(
    '#type' => 'fieldset',
    '#title' => t('New Style'),
    '#collapsible' => FALSE);

  $form["emailmonkey_style_new"]['emailmonkey_style_name_new'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A descriptive name for the style.'),
    '#size' => 45,
    '#maxlength' => 45,
    '#element_validate' => array('emailmonkey_form_validate_new_style_name'));

  $form["emailmonkey_style_new"]['emailmonkey_style_path_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Style sheet path'),
      '#description' => t('Path to the style sheet relative to the Drupal root.'),
      '#size' => 100,
      '#element_validate' => array('emailmonkey_form_validate_new_style_path'));

  $form['actions'] = array(
    '#type' => 'actions',
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Save')));

  return $form;
}

/**
 * Validate forms submitted for configuring styles.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_configure_styles_validate($form, &$form_state) {
  $paths = array();
  $offset = 0;

  while (isset($form_state['values']["emailmonkey_style_machine_name_{$offset}"])) {
    $path = $form_state['values']["emailmonkey_style_path_{$offset}"];

    if (!$form_state['values']["emailmonkey_style_delete_{$offset}"]) {
      if (isset($paths[$path])) {
        form_set_error("emailmonkey_style_path_{$offset}", t('The style sheet path is already in use.'));
      }

      $paths[$path] = TRUE;
    }

    $offset += 1;
  }

  $new_path = $form_state['values']['emailmonkey_style_path_new'];

  if (!empty($new_path) && isset($paths[$new_path])) {
    form_set_error("emailmonkey_style_path_new", t('The style sheet path is already in use.'));
  }
}

/**
 * Handle forms submitted for configuring styles.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_configure_styles_submit($form, &$form_state) {
  $styles = array();
  $offset = 0;

  while (isset($form_state['values']["emailmonkey_style_machine_name_{$offset}"])) {
    $machine_name = $form_state['values']["emailmonkey_style_machine_name_{$offset}"];
    $name = $form_state['values']["emailmonkey_style_name_{$offset}"];
    $path = $form_state['values']["emailmonkey_style_path_{$offset}"];

    if ($form_state['values']["emailmonkey_style_delete_{$offset}"]) {
      emailmonkey_data_remove_style($machine_name);
    }
    else {
      $styles[$machine_name] = array('machine_name' => $machine_name, 'name' => $name, 'path' => $path);
    }

    $offset += 1;
  }

  if (!empty($form_state['values']['emailmonkey_style_name_new'])) {
    $name = $form_state['values']['emailmonkey_style_name_new'];
    $path = $form_state['values']['emailmonkey_style_path_new'];
    $machine_name = emailmonkey_create_machine_name($name);

    if (isset($styles[$machine_name])) {
      $alternate_machine_name = "{$machine_name}_2";

      for ($i = 3; isset($styles[$machine_name]); $i++) {
        $alternate_machine_name = "{$machine_name}_{$i}";
      }

      $machine_name = $alternate_machine_name;
    }

    $styles[$machine_name] = array('machine_name' => $machine_name, 'name' => $name, 'path' => $path);
  }

  if (variable_get('emailmonkey_styles') != $styles) {
    drupal_set_message(t('Successfully updated the styles.'));
    variable_set('emailmonkey_styles', $styles);
  }
}

function emailmonkey_form_field_node_id($node) {
  return array(
    '#type' => 'hidden',
    '#value' => $node->nid,
    '#element_validate' => array('emailmonkey_form_validate_node_id'));
}

/**
 * Create a form for deleting campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_delete_campaign($form, &$form_state, $node) {
  $form['node_id'] = emailmonkey_form_field_node_id($node);
  $form['node_id']['#element_validate'][] = 'emailmonkey_form_validate_node_id_has_campaign';

  $form['confirm'] = array(
    '#markup' => '<p>'.t('Are you sure you want to delete the campaign for %title?', array('%title' => $node->title)).'</p>');

  $form['delete_from_drupal_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete from Drupal only'),
    '#description' => t('Delete the campaign reference in Drupal but don\'t delete the campaign in MailChimp.'),
    '#default_value' => true);

  $form['actions'] = array(
    '#type' => 'actions',
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Delete')));

  return $form;
}

/**
 * Handle forms submitted for deleting campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_delete_campaign_submit($form, &$form_state) {
  $node = node_load($form_state['values']['node_id']);
  $delete_from_drupal_only = $form_state['values']['delete_from_drupal_only'];
  $account_id = variable_get('emailmonkey_account_id');

  if (!$delete_from_drupal_only) {
    try {
      emailmonkey_campaigns_delete_campaign_for_node($node, $account_id);
    }
    catch (EmailMonkey_Exception $e) {
      drupal_set_message(t('Failed to delete the campaign from MailChimp. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
      return;
    }
  }

  emailmonkey_data_unlink_node_from_campaign($node, $account_id);
  $form_state['redirect'] = emailmonkey_node_url($node, 'preview');

  if ($delete_from_drupal_only) {
    drupal_set_message(t('Successfully deleted the campaign from Drupal.'));
  }
  else {
    drupal_set_message(t('Successfully deleted the campaign from MailChimp.'));
  }
}

/**
 * Create a form for exporting campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_export_campaign($form, &$form_state, $node, $lists) {
  $selected_list_id = !empty($form_state['values']['list']) ? $form_state['values']['list'] : NULL;
  $selected_list = NULL;
  $is_list_selected = !empty($selected_list_id);
  $list_options = array();
  $segment_options = array();
  $max_segments = 250;

  foreach ($lists->lists as $list) {
    $list_options[$list->id] = $list->name;

    if ($list->id == $selected_list_id) {
      $selected_list = $list;
    }
  }

  if ($is_list_selected) {
    // It would be better to load segments externally and use dependency
    // injection to get them here, but the Drupal form API doesn't make it
    // particularly easy to do that, especially with regards to building forms,
    // form arguments, and caching.

    try {
      // First, get the total so that an offset can be computed below to get the
      // newest $max_segments amount of segments.
      $total_segments = emailmonkey_segments_get_segment_count($selected_list->id);
    }
    catch (EmailMonkey_Exception $e) {
      drupal_set_message(t('Failed to get MailChimp list segment total. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
      return array();
    }

    if ($total_segments > 0) {
      $offset = max(0, $total_segments-$max_segments);

      try {
        $segments = emailmonkey_segments_get_segments_basics($selected_list->id, $max_segments, $offset);
      }
      catch (EmailMonkey_Exception $e) {
        drupal_set_message(t('Failed to get MailChimp list segments. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
        return array();
      }

      // The API doesn't provide a way to sort the segments, so they need to be
      // added to the list of options in reverse order to make sure the newest
      // segments appear first.
      for ($i = count($segments->segments) - 1; $i >= 0; $i--) {
        $segment = $segments->segments[$i];
        $num_members = format_plural($segment->member_count, '1 member', '@count members');
        $label = t('@segment (@num-members)', array('@segment' => $segment->name, '@num-members' => $num_members));
        $segment_options[$segment->id] = htmlspecialchars_decode($label);
      }
    }

    $selected_list_from_name = $selected_list->campaign_defaults->from_name;
    $selected_list_reply_to = $selected_list->campaign_defaults->from_email;
  }
  else {
    $selected_list_from_name = NULL;
    $selected_list_reply_to = NULL;
    $total_segments = 0;
  }

  $form['node_id'] = emailmonkey_form_field_node_id($node);

  $form['list'] = array(
    '#type' => 'select',
    '#title' => t('List'),
    '#options' => $list_options,
    '#multiple' => FALSE,
    '#required' => TRUE,
    '#access' => !$is_list_selected,
    '#description' => t('Select the MailChimp list to associate with the campaign.'),
    '#element_validate' => array('emailmonkey_form_validate_list_id'));

  if ($is_list_selected) {
    $form['list']['#value'] = $selected_list->id;
  }

  $form['segment'] = array(
    '#type' => 'select',
    '#title' => t('Segment'),
    '#options' => $segment_options,
    '#multiple' => FALSE,
    '#empty_value' => -1,
    '#access' => $is_list_selected && !empty($segment_options),
    '#description' => t('An optional MailChimp list segment to use in order to send the campaign to a subset of the list.'));

  if ($is_list_selected && $total_segments > count($segment_options)) {
    $form['segment']['#description'] .= ' ' . t('<strong>More segments exist for the list than can be displayed here</strong>. After exporting, you\'ll need to use the official MailChimp interface to select a segment that isn\'t displayed here.');
  }

  $form['campaign_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign name'),
    '#description' => t('The optional internal/administrative name for the campaign. This is only used to identify the campaign in the MailChimp user interface.'),
    '#default_value' => '',
    '#access' => $is_list_selected);

  $form['subject_line'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject line'),
    '#description' => t('The subject line to use when sending the campaign. The node title is used as the default here.'),
    '#default_value' => $node->title,
    '#access' => $is_list_selected);

  $form['from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('From name'),
    '#description' => t('The from name to use when sending the campaign. The list default is used as the default here.'),
    '#default_value' => $selected_list_from_name,
    '#access' => $is_list_selected);

  $form['reply_to'] = array(
    '#type' => 'textfield',
    '#title' => t('From/Reply-to email address'),
    '#description' => t('The from/reply-to email address to use when sending the campaign. The list default is used as the default here.'),
    '#default_value' => $selected_list_reply_to,
    '#access' => $is_list_selected,
    '#element_validate' => array('emailmonkey_form_validate_email_address'));

  if (emailmonkey_user_access('emailmonkey_send_campaigns')) {
    $form['test'] = array(
      '#type' => 'checkbox',
      '#title' => t('Test campaign'),
      '#access' => $is_list_selected,
      '#description' => t('Test the new campaign after exporting.'));
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'continue' => array(
      '#type' => 'button',
      '#value' => t('Continue...'),
      '#access' => !$is_list_selected),
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Export'),
      '#access' => $is_list_selected));

  return $form;
}

/**
 * Handle forms submitted for exporting campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_export_campaign_submit($form, &$form_state) {
  $node = node_load($form_state['values']['node_id']);
  $account_id = variable_get('emailmonkey_account_id');

  $list_id = $form_state['values']['list'];
  $segment_id = !empty($form_state['values']['segment']) ? $form_state['values']['segment'] : NULL;
  $from_name = trim($form_state['values']['from_name']);
  $reply_to = trim($form_state['values']['reply_to']);
  $campaign_name = trim($form_state['values']['campaign_name']);
  $subject = trim($form_state['values']['subject_line']);
  $body = emailmonkey_render_raw_node_with_wrapper($node);

  if ($segment_id == '-1') {
    $segment_id = NULL;
  }

  if (strlen($campaign_name) == 0) {
    $campaign_name = NULL;
  }

  try {
    $list = emailmonkey_lists_get_list_basics($list_id);
  }
  catch (EmailMonkey_Exception $e) {
    drupal_set_message(t('Failed to get MailChimp list information. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
    return;
  }

  try {
    $campaign = emailmonkey_campaigns_create_basic_campaign($list_id, $segment_id, $subject, $from_name, $reply_to, $body, $campaign_name);
  }
  catch (EmailMonkey_Exception $e) {
    drupal_set_message(t('Failed to export the campaign to MailChimp. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
    return;
  }

  emailmonkey_data_link_node_with_campaign($node, $account_id, $campaign);
  drupal_set_message(t('Successfully exported the campaign to MailChimp.'));

  if ($form_state['values']['test']) {
    $form_state['redirect'] = emailmonkey_node_url($node, 'test');
  }
  else {
    $form_state['redirect'] = emailmonkey_node_url($node, 'preview');
  }
}

/**
 * Create a form for sending campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_send_campaign($form, &$form_state, $node, $campaign, $segment) {
  $form['node_id'] = emailmonkey_form_field_node_id($node);
  $form['node_id']['#element_validate'][] = 'emailmonkey_form_validate_node_id_has_campaign';

  $form['confirm'] = array(
    '#markup' => '<p>'.t('Are you sure you want to send the campaign for %title?', array('%title' => $node->title)).'</p>');

  $list = array('title' => NULL, 'type' => 'ul', 'attributes' => array());

  $campaign_list = $list;

  if (!empty($campaign->settings->title)) {
    $campaign_list['items'][] = t('Campaign name: %name', array('%name' => $campaign->settings->title));
  }

  $campaign_list['items'][] = t('Subject: %subject', array('%subject' => $campaign->settings->subject_line));
  $campaign_list['items'][] = t('Sender\'s name: %name', array('%name' => $campaign->settings->from_name));
  $campaign_list['items'][] = t('Sender\'s email address: %email', array('%email' => $campaign->settings->reply_to));

  $form['campaign_information'] = array(
    '#markup' => '<p>'.t('Campaign information:').'</p>'.theme_item_list($campaign_list));

  $recipient_list = $list;
  $recipient_list['items'][] = t('List: %list', array('%list' => $campaign->recipients->list_name));

  if (!empty($segment)) {
    $num_members = format_plural($segment->member_count, '1 member', '@count members');
    $recipient_list['items'][] = t('Segment: %segment (@num-members)', array('%segment' => $segment->name, '@num-members' => $num_members));
  }

  // Only display additional recipient information if there isn't a segment or
  // if the segment is static. The additional information for static segments is
  // redundant.
  if (empty($segment) || $segment->type != 'static') {
    $segment_test = filter_xss($campaign->recipients->segment_text);

    if (!empty($segment_text)) {
      $recipient_list['items'][] = $segment_text;
    }
  }

  $form['recipient_information'] = array(
    '#markup' => '<p>'.t('Recipient information:').'</p>'.theme_item_list($recipient_list));

  $form['update_campaign'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update campaign'),
    '#default_value' => TRUE,
    '#description' => t('Prior to sending, update MailChimp with any changes to the content, styles, or templates that occurred since the last export or update.'));

  $form['actions'] = array(
    '#type' => 'actions',
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Send')));

  return $form;
}

/**
 * Handle forms submitted for sending campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_send_campaign_submit($form, &$form_state) {
  $node = node_load($form_state['values']['node_id']);
  $account_id = variable_get('emailmonkey_account_id');
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  if ($form_state['values']['update_campaign']) {
    $content = emailmonkey_render_raw_node_with_wrapper($node);

    try {
      emailmonkey_campaigns_set_campaign_content($campaign_id, $content);
    }
    catch (EmailMonkey_Exception $e) {
      drupal_set_message(t('Failed to update and send the campaign. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
      return;
    }
  }

  try {
    emailmonkey_campaigns_send_campaign($campaign_id);
  }
  catch (EmailMonkey_Exception $e) {
    drupal_set_message(t('Failed to send the campaign. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
    return;
  }

  $form_state['redirect'] = emailmonkey_node_url($node, 'preview');
  drupal_set_message(t('Successfully sent the campaign.'));
}

/**
 * Create a form for testing campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_test_campaign($form, &$form_state, $node) {
  global $user;

  $form['node_id'] = emailmonkey_form_field_node_id($node);
  $form['node_id']['#element_validate'][] = 'emailmonkey_form_validate_node_id_has_campaign';

  $form['confirm'] = array(
    '#markup' => '<p>'.t('Are you sure you want to test the campaign for %title?', array('%title' => $node->title)).'</p>');

  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#description' => t('The email addresses, one per line, of the recipients to whom to send the test.'),
    '#required' => TRUE,
    '#default_value' => $user->mail . "\n",
    '#element_validate' => array('emailmonkey_form_validate_textual_email_list'));

  $form['send_using_drupal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send using Drupal'),
    '#default_value' => FALSE,
    '#description' => t('Send the test using Drupal\'s mail system instead of the MailChimp API to avoid hitting testing limits. MailChimp merge tags won\'t work, and some may be altered to prevent HTML formatting errors. It is best to enable this when doing initial tests and to disable it when testing immediately prior to sending.'));

  $form['update_campaign'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update campaign'),
    '#default_value' => TRUE,
    '#states' => array(
      'invisible' => array(':input[name="send_using_drupal"]' => array('checked' => TRUE))),
    '#description' => t('Prior to sending the test, update MailChimp with any changes to the content, styles, or templates that occurred since the last export or update.'));

  $form['actions'] = array(
    '#type' => 'actions',
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Test')));

  return $form;
}

/**
 * Handle forms submitted for testing campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_test_campaign_submit($form, &$form_state) {
  $node = node_load($form_state['values']['node_id']);
  $account_id = variable_get('emailmonkey_account_id');
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  if ($form_state['values']['send_using_drupal']) {
    $params['node'] = $node;
    $recipients = strtr(trim($form_state['values']['recipients']), "\n", ',');
    $result_message = emailmonkey_html_mail('emailmonkey', 'test', $recipients, language_default(), $params);

    if (!$result_message['result']) {
      drupal_set_message(t('Failed to send the test. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
      return;
    }
  }
  else {
    $recipients = explode("\n", trim($form_state['values']['recipients']));

    if ($form_state['values']['update_campaign']) {
      $content = emailmonkey_render_raw_node_with_wrapper($node);

      try {
        emailmonkey_campaigns_set_campaign_content($campaign_id, $content);
      }
      catch (EmailMonkey_Exception $e) {
        drupal_set_message(t('Failed to update and test the campaign. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
        return;
      }
    }

    try {
      emailmonkey_campaigns_test_campaign($campaign_id, $recipients);
    }
    catch (EmailMonkey_Exception $e) {
      drupal_set_message(t('Failed to send the test. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
      return;
    }
  }

  $form_state['redirect'] = emailmonkey_node_url($node, 'preview');
  drupal_set_message(t('Successfully sent the test.'));
}

/**
 * Create a form for updating campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_update_campaign($form, &$form_state, $node) {
  $form['node_id'] = emailmonkey_form_field_node_id($node);
  $form['node_id']['#element_validate'][] = 'emailmonkey_form_validate_node_id_has_campaign';

  $form['confirm'] = array(
    '#markup' => '<p>'.t('Are you sure you want to update the campaign for %title? This will update the campaign in MailChimp with any changes to the content, styles, or templates that occurred since the last export or update.', array('%title' => $node->title)).'</p>');

  $form['actions'] = array(
    '#type' => 'actions',
    'export' => array(
      '#type' => 'submit',
      '#value' => t('Update')));

  return $form;
}

/**
 * Handle forms submitted for updating campaigns.
 *
 * @see drupal_get_form()
 */
function emailmonkey_form_update_campaign_submit($form, &$form_state) {
  $node = node_load($form_state['values']['node_id']);
  $account_id = variable_get('emailmonkey_account_id');
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  $content = emailmonkey_render_raw_node_with_wrapper($node);

  try {
    emailmonkey_campaigns_set_campaign_content($campaign_id, $content);
  }
  catch (EmailMonkey_Exception $e) {
    drupal_set_message(t('Failed to update the campaign body. Check the <a href="@log_url">log</a> for more information.', array('@log_url' => url('admin/reports/dblog'))), 'error');
    return;
  }

  $form_state['redirect'] = emailmonkey_node_url($node, 'preview');
  drupal_set_message(t('Successfully updated the campaign.'));
}

function emailmonkey_form_validate_api_key($element, &$form_state, $form) {
  $api_key = trim($element['#value']);

  if (empty($api_key)) {
    form_error($element, t('The %title field is required.', array('%title' => $element['#title'])));
  }
  elseif (!emailmonkey_validation_is_api_key($api_key)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
  elseif (!emailmonkey_validation_is_valid_api_key($api_key)) {
    form_error($element, t('The %title field is invalid.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_list_id($element, &$form_state, $form) {
  $list_id = $element['#value'];

  if (!emailmonkey_validation_is_list_id($list_id)) {
    var_dump($list_id);
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }

  $is_valid_list_id = emailmonkey_validation_is_valid_list_id($list_id);

  if ($is_valid_list_id === NULL) {
    form_error($element, t('The %title field is can\'t be validated because the library dependencies aren\'t installed.', array('%title' => $element['#title'])));
  }
  elseif ($is_valid_list_id === FALSE) {
    form_error($element, t('The %title field is invalid.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_campaign_id($element, &$form_state, $form) {
  $campaign_id = $element['#value'];

  if (!emailmonkey_validation_is_campaign_id($campaign_id)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }

  $is_valid_campaign_id = emailmonkey_validation_is_valid_campaign_id($campaign_id);

  if ($is_valid_campaign_id === NULL) {
    form_error($element, t('The %title field is can\'t be validated because the library dependencies aren\'t installed.', array('%title' => $element['#title'])));
  }
  elseif ($is_valid_campaign_id === FALSE) {
    form_error($element, t('The %title field is invalid.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_node_id($element, &$form_state, $form) {
  $node_id = $element['#value'];

  if (!node_load($node_id)) {
    form_error($element, t('The node does not exist.'));
  }
}

function emailmonkey_form_validate_node_id_has_campaign($element, &$form_state, $form) {
  $node_id = $element['#value'];
  $node = node_load($node_id);
  $account_id = variable_get('emailmonkey_account_id');

  if (!$node) {
    form_error($element, t('The node does not exist.'));
  }
  elseif (!emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    form_error($element, t('The node hasn\'t been exported yet.'));
  }
}

function emailmonkey_form_validate_new_style_name($element, &$form_state, $form) {
  $style_name = $element['#value'];

  if (!empty($style_name) && !emailmonkey_validation_is_style_name($style_name)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_existing_style_name($element, &$form_state, $form) {
  $style_name = $element['#value'];

  if (!emailmonkey_validation_is_style_name($style_name)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_style_name($element, &$form_state, $form) {
  $style_name = $element['#value'];

  if (!emailmonkey_validation_is_style_name($style_name)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
  elseif (!emailmonkey_validation_is_valid_style_name($style_name)) {
    form_error($element, t('The %title field is invalid.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_new_style_path($element, &$form_state, $form) {
  $style_path = $element['#value'];
  $full_path = DRUPAL_ROOT . '/' . $style_path;

  if (!empty($style_path) && (!file_exists($full_path) || is_dir($full_path))) {
    form_error($element, t('The %title field is not a valid path.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_style_path($element, &$form_state, $form) {
  $style_path = $element['#value'];
  $full_path = DRUPAL_ROOT . '/' . $style_path;

  if (!file_exists($full_path) || is_dir($full_path)) {
    form_error($element, t('The %title field is not a valid path.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_email_address($element, &$form_state, $form) {
  $email = $element['#value'];

  if (!empty($email) && !emailmonkey_validation_is_email_address(trim($email))) {
    form_error($element, t('The %title field doesn\'t appear to be an email address.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_textual_email_list($element, &$form_state, $form) {
  $email_list = preg_split('/[\\r\\n]+/', trim($element['#value']));

  if (!emailmonkey_validation_is_array_of_email_addresses($email_list)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
}

function emailmonkey_form_validate_machine_name($element, &$formstate_, $form) {
  $machine_name = $element['#value'];

  if (!emailmonkey_validation_is_machine_name($machine_name)) {
    form_error($element, t('The %title field is not in the correct format.', array('%title' => $element['#title'])));
  }
}
