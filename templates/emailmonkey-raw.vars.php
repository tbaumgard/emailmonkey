<?php
/**
 * @file
 * File for "emailmonkey_raw" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_raw" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_raw(&$variables) {
  $node = node_load(arg(1));
  $node_data = emailmonkey_data_node_data_for_node($node);

  $variables['node'] = $node;
  $variables['error'] = FALSE;
  $variables['error_message'] = '';

  if (!empty($node_data)) {
    try {
      $raw_node = emailmonkey_render_raw_node($node);
    }
    catch (EmailMonkey_Exception $e) {
      $varaibles['error'] = TRUE;
      $variables['error_message'] = $e->getMessage();
      return;
    }

    $view_mode = $node_data['view_mode'];
    $module_path = drupal_get_path('module', 'emailmonkey');
    $all_styles = variable_get('emailmonkey_styles', array());
    $styles = emailmonkey_data_get_styles_for_content_type_view_mode($node->type, $view_mode);
    $css = file_get_contents("{$module_path}/css/email-template.css");

    foreach ($styles as $machine_name) {
      if (!empty($all_styles[$machine_name]['path'])) {
        $css .= @file_get_contents($all_styles[$machine_name]['path']);
      }
    }

    $variables['css'] = $css;
    $variables['rendered_node'] = $raw_node;
  }
  else {
    $node_uri = node_uri($node);
    $edit_url = url($node_uri['path'].'/edit');

    $variables['error'] = TRUE;
    $variables['error_message'] = t('This node doesn\'t have a email view mode set and therefore can\'t be rendered. <a href="@edit-link">Edit the node</a> to set one.', array('@edit-link' => $edit_url));
  }
}
