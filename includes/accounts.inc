<?php

/**
 * @file
 * Items used for MailChimp accounts.
 */

// -----------------------------------------------------------------------------
// Constants

define('EMAILMONKEY_BASIC_ACCOUNT_FIELDS', array(
  'account_id', 'account_name', 'email', 'role'));

// -----------------------------------------------------------------------------
// Functions

function emailmonkey_accounts_get_account_basics() {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_ACCOUNT_FIELDS));

  return emailmonkey_api('root', 'getAccount', $parameters);
}
