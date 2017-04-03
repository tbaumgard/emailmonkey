<?php
/**
 * @file
 * File for "emailmonkey_preview" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_preview" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_preview(&$variables) {
  $node = $variables['node'];
  $node_uri = node_uri($node);
  $node_data = emailmonkey_data_node_data_for_node($node);

  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (empty($node_data['view_mode'])) {
    $edit_url = url($node_uri['path'].'/edit');
    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node doesn\'t have a email view mode set. <a href="@edit-link">Edit the node</a> to set one.', array('@edit-link' => $edit_url));
  }
  else {
    $variables['raw_preview_url'] = url(emailmonkey_node_url($node, 'raw'));
  }
}
