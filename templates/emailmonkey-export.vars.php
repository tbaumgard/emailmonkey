<?php
/**
 * @file
 * File for "emailmonkey_export" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_export" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_export(&$variables) {
  $node = $variables['node'];
  $account_id = variable_get('emailmonkey_account_id');

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (!emailmonkey_data_is_node_linked_with_campaign($node, $account_id)) {
    try {
      $lists = emailmonkey_lists_get_lists_basics();
    }
    catch (EmailMonkey_Exception $e) {
      $variables['error'] = TRUE;
      $variables['error_message'] = t('An error occurred and the export form couldn\'t be generated.');
      return;
    }

    if (!empty($_POST['list'])) {
      foreach ($lists->lists as $list) {
        if ($list->id == $_POST['list']) {
          $list_id = $list->id;
          break;
        }
      }
    }

    try {
      $variables['form'] = drupal_get_form('emailmonkey_form_export_campaign', $node, $lists);
    }
    catch (EmailMonkey_Exception $e) {
      $variables['error'] = TRUE;
      $variables['error_message'] = t('Failed to load the export form.');
    }
  }
  else {
    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node has already been exported.');
    return;
  }
}
