<?php
/**
 * @file
 * File for "emailmonkey_send" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_send" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_send(&$variables) {
  $node = $variables['node'];
  $node_data = emailmonkey_data_node_data_for_node($node);
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (!emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('Nodes must be exported before sending.');
    return;
  }

  $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

  try {
    $campaign = emailmonkey_campaigns_get_campaign_basics($campaign_id);
  }
  catch (EmailMonkey_Exception $e) {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('Couldn\'t retrieve campaign details from MailChimp.');
    return;
  }

  if (!empty($campaign->recipients->segment_opts->saved_segment_id)) {
    $list_id = $campaign->recipients->list_id;
    $segment_id = $campaign->recipients->segment_opts->saved_segment_id;

    try {
      $segment = emailmonkey_segments_get_segment_basics($list_id, $segment_id);
    }
    catch (EmailMonkey_Exception $e) {
      $variables['error'] = TRUE;
      $variables['error_message'] = t('Couldn\'t retrieve list segment details from MailChimp.');
      return;
    }
  } else {
    $segment = NULL;
  }

  if (!emailmonkey_campaigns_is_sent_status($campaign->status)) {
    $variables['form'] = drupal_get_form('emailmonkey_form_send_campaign', $node, $campaign, $segment);
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('The campaign has already been sent.');
  }
}
