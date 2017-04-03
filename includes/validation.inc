<?php

/**
 * @file
 * Functions used to handle validation.
 */

/**
 * Determine if a value is a syntactically valid MailChimp API key.
 *
 * @param string $api_key
 *   The API key to test.
 *
 * @return
 *   Returns TRUE when the API key is syntactically valid and FALSE otherwise.
 */
function emailmonkey_validation_is_api_key($api_key) {
  // Be somewhat lenient with the data center portion.
  return preg_match('/^[0-9a-f]{32}-[a-z]{1,5}\\d{1,5}$/', $api_key) == 1;
}

/**
 * Determine if a value is a valid MailChimp API key.
 *
 * @param string $api_key
 *   The API key to test.
 *
 * @return
 *   Returns TRUE when the API key is valid and FALSE otherwise.
 */
function emailmonkey_validation_is_valid_api_key($api_key) {
  // This function needs to use lower-level method calls than what should be
  // used typically since it's likely that it's being used before the module
  // has been fully configured.

  try {
    emailmonkey_load_libraries();
  }
  catch (EmailMonkey_Exception $e) {
    return FALSE;
  }

  $mc = new Mailchimp\Mailchimp($api_key);

  try {
    $mc->getAccount();
    return TRUE;
  }
  catch (Mailchimp\MailchimpAPIException $e) {
    return FALSE;
  }
}

/**
 * Determine if a value is a syntactically valid MailChimp list ID.
 *
 * @param string $list_id
 *   The list ID to test.
 *
 * @return
 *   Returns TRUE when the list ID is syntactically valid and FALSE otherwise.
 */
function emailmonkey_validation_is_list_id($list_id) {
  // Be somewhat lenient for backwards and future compatibility.
  return preg_match('/^[0-9a-f]{5,15}$/', $list_id) == 1;
}

/**
 * Determine if a value is a valid MailChimp list.
 *
 * @param string $list_id
 *   The list ID to test.
 *
 * @return
 *   Returns TRUE when the list ID is valid and FALSE otherwise.
 */
function emailmonkey_validation_is_valid_list_id($list_id) {
  try {
    // Limit the results to just the ID so that time isn't wasted sending all of
    // the details back. Also, this isn't perfect: errors in the API calls will
    // be interpreted as if the call succeeded but the list ID is invalid.
    emailmonkey_api('lists', 'getList', $list_id, array('fields' => 'id'));
    return TRUE;
  }
  catch (EmailMonkey_Exception $e) {
    return emailmonkey_are_libraries_installed() ? FALSE : NULL;
  }
}

function emailmonkey_validation_is_campaign_id($campaign_id) {
  // Be somewhat lenient for backwards and future compatibility.
  return preg_match('/^[0-9a-f]{5,15}$/', $campaign_id) == 1;
}

function emailmonkey_validation_is_valid_campaign_id($campaign_id) {
  try {
    // Limit the results to just the ID so that time isn't wasted sending all of
    // the details back.
    emailmonkey_api('campaigns', 'getCampaign', $campaign_id, array('fields' => 'id'));
    return TRUE;
  }
  catch (EmailMonkey_Exception $e) {
    return emailmonkey_are_libraries_installed() ? FALSE : NULL;
  }
}

function emailmonkey_validation_is_style_name($style_name) {
  return strlen(trim($style_name)) > 0;
}

function emailmonkey_validation_is_valid_style_name($style_name) {
  $styles = variable_get('emailmonkey_styles', array());
  return !empty($styles[$style_name]);
}

function emailmonkey_validation_is_email_address($email) {
  return filter_var($email, FILTER_VALIDATE_EMAIL);
}

function emailmonkey_validation_is_array_of_email_addresses($addresses) {
  if (!is_array($addresses)) {
    return FALSE;
  }

  foreach ($addresses as $address) {
    if (!emailmonkey_validation_is_email_address($address)) {
      return FALSE;
    }
  }

  return TRUE;
}

function emailmonkey_validation_is_machine_name($machine_name) {
  return preg_match('/^[a-z0-9_]+$/', $machine_name) == 1;
}
