<?php
/**
 * @file
 * File for "emailmonkey_update" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_update" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_update(&$variables) {
  $node = $variables['node'];
  $node_data = emailmonkey_data_node_data_for_node($node);
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (!emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('Nodes must be exported before updating.');
    return;
  }

  if (empty($node_data)) {
    $node_uri = node_uri($node);
    $edit_url = url($node_uri['path'].'/edit');

    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node doesn\'t have a email view mode set. <a href="@edit-link">Edit the node</a> to set one.', array('@edit-link' => $edit_url));
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

  if (!emailmonkey_campaigns_is_sent_status($campaign->status)) {
    $variables['form'] = drupal_get_form('emailmonkey_form_update_campaign', $node);
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('The campaign has already been sent.');
  }
}
