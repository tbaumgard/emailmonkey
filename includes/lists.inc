<?php

/**
 * @file
 * Items used for MailChimp lists.
 */

// -----------------------------------------------------------------------------
// Constants

define('EMAILMONKEY_BASIC_LISTS_FIELDS', array(
  'lists.id', 'lists.name', 'lists.campaign_defaults.from_name',
  'lists.campaign_defaults.from_email', 'total_items'));

define('EMAILMONKEY_BASIC_LIST_FIELDS', array(
  'id', 'name', 'campaign_defaults.from_name', 'campaign_defaults.from_email'));

// -----------------------------------------------------------------------------
// Functions

function emailmonkey_lists_get_lists_basics() {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_LISTS_FIELDS));

  return emailmonkey_api('lists', 'getLists', $parameters);
}

function emailmonkey_lists_get_list_basics($list_id) {
  $parameters = array('fields' => implode(',', EMAILMONKEY_BASIC_LIST_FIELDS));

  return emailmonkey_api('lists', 'getList', $list_id, $parameters);
}
