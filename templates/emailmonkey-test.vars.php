<?php
/**
 * @file
 * File for "emailmonkey_test" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_test" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_test(&$variables) {
  $node = $variables['node'];
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (!emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('Nodes must be exported before testing.');
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
    $variables['form'] = drupal_get_form('emailmonkey_form_test_campaign', $node);
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('The campaign has already been sent.');
  }
}
