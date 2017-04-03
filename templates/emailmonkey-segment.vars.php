<?php
/**
 * @file
 * File for "emailmonkey_segment" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_segment" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_segment(&$variables) {
  $node = $variables['node'];
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    $campaign_id = emailmonkey_data_linked_campaign_id_for_node($node, $account_id);

    try {
      $campaign = emailmonkey_campaigns_get_campaign_basics($campaign_id);
    }
    catch (EmailMonkey_Exception $e) {
      $variables['error'] = TRUE;
      $variables['error_message'] = t('Couldn\'t retrieve campaign details from MailChimp.');
      return;
    }

    $list_id = $campaign->recipients->list_id;

    try {
      $lists = emailmonkey_lists_get_lists_basics();
      $variables['form'] = drupal_get_form('emailmonkey_form_export_campaign', $node, $lists);
    }
    catch (EmailMonkey_Exception $e) {
      $variables['error'] = TRUE;
      $variables['error_message'] = t('An error occurred and the export form couldn\'t be generated.');
    }
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node hasn\'t been exported yet.');
  }
}
