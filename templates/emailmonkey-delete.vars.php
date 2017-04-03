<?php
/**
 * @file
 * File for "emailmonkey_delete" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_delete" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_delete(&$variables) {
  $node = $variables['node'];
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    $variables['form'] = drupal_get_form('emailmonkey_form_delete_campaign', $node);
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node can\'t be deleted because it hasn\'t been exported yet.');
  }
}
