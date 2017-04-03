<?php

/**
 * @file
 * Items used for EmailMonkey database data.
 */

function emailmonkey_data_has_data_for_node($node) {
  $query = db_select('emailmonkey_node_data', 'data');
  $query->addExpression('COUNT(*)', 'row_count');
  $query->condition('data.node_id', $node->nid);

  return $query->execute()->fetchField() != 0;
}

function emailmonkey_data_is_node_linked_with_campaign($node, $account_id) {
  $query = db_select('emailmonkey_campaigns', 'campaigns');
  $query->addExpression('COUNT(*)', 'row_count');
  $query->condition('campaigns.node_id', $node->nid);
  $query->condition('campaigns.account_id', $account_id);

  return $query->execute()->fetchField() != 0;
}

function emailmonkey_data_link_node_with_campaign($node, $account_id, $campaign) {
  if (emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    db_update('emailmonkey_campaigns')
      ->fields(array('campaign_id' => $campaign->id))
      ->condition('node_id', $node->nid)
      ->condition('account_id', $account_id)
      ->execute();
  }
  else {
    db_insert('emailmonkey_campaigns')
      ->fields(array(
        'node_id' => $node->nid,
        'account_id' => $account_id,
        'campaign_id' => $campaign->id))
      ->execute();
  }
}

function emailmonkey_data_unlink_node_from_campaign($node, $account_id) {
  db_delete('emailmonkey_campaigns')
    ->condition('node_id', $node->nid)
    ->condition('account_id', $account_id)
    ->execute();
}

function emailmonkey_data_linked_campaign_id_for_node($node, $account_id) {
  $campaign_id = db_select('emailmonkey_campaigns', 'campaigns')
    ->fields('campaigns', array('campaign_id'))
    ->condition('campaigns.node_id', $node->nid)
    ->condition('campaigns.account_id', $account_id)
    ->execute()
    ->fetchField();

  return $campaign_id !== FALSE ? $campaign_id : NULL;
}

function emailmonkey_data_node_data_for_node($node) {
  return db_select('emailmonkey_node_data', 'data')
    ->fields('data')
    ->condition('data.node_id', $node->nid)
    ->execute()
    ->fetchAssoc();
}

function emailmonkey_data_set_node_data_for_node($node, $data) {
  $valid_keys = array('view_mode');
  $values = array();

  foreach ($valid_keys as $valid_key) {
    if (array_key_exists($valid_key, $data)) {
      $values[$valid_key] = $data[$valid_key];
    }
  }

  if (emailmonkey_data_has_data_for_node($node)) {
    db_update('emailmonkey_node_data')
      ->fields($values)
      ->condition('node_id', $node->nid)
      ->execute();
  }
  else {
    $values['node_id'] = $node->nid;

    db_insert('emailmonkey_node_data')
      ->fields($values)
      ->execute();
  }
}

function emailmonkey_data_remove_node_data_for_node($node) {
  db_delete('emailmonkey_node_data')
    ->condition('node_id', $node->nid)
    ->execute();
}

function emailmonkey_data_get_styles_for_content_type_view_mode($content_type, $view_mode) {
  return db_select('emailmonkey_styles', 'styles')
    ->fields('styles', array('style'))
    ->condition('styles.content_type', $content_type)
    ->condition('styles.view_mode', $view_mode)
    ->execute()
    ->fetchCol();
}

function emailmonkey_data_update_styles_for_content_type_view_mode($content_type, $view_mode, $styles) {
  db_delete('emailmonkey_styles')
    ->condition('content_type', $content_type)
    ->condition('view_mode', $view_mode)
    ->execute();

  foreach ($styles as $style) {
    $fields = array(
      'content_type' => $content_type,
      'view_mode' => $view_mode,
      'style' => $style);

    db_insert('emailmonkey_styles')
      ->fields($fields)
      ->execute();
  }
}

function emailmonkey_data_remove_style($style) {
  db_delete('emailmonkey_styles')
    ->condition('style', $style)
    ->execute();
}

function emailmonkey_data_sync_node_data($node, $account_id) {
  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  if (!empty($campaign_id)) {
    if (emailmonkey_validation_is_valid_campaign_id($campaign_id) === FALSE) {
      emailmonkey_data_unlink_node_from_campaign($node, $account_id);
    }
  }
}
