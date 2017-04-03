<?php
/**
 * @file
 * File for "emailmonkey_help" theme hook (pre-)process functions.
 */

/**
 * Pre-processes variables for the "emailmonkey_help" theme hook.
 *
 * @ingroup theme_preprocess
 */
function emailmonkey_preprocess_emailmonkey_help(&$variables) {
  $module_path = drupal_get_path('module', 'emailmonkey');

  $variables['permissions_url'] = url('admin/people/permissions', array('fragment' => 'module-emailmonkey'));
  $variables['content_types_url'] = url('admin/structure/types');
  $variables['configuration_url'] = url('admin/config/services/emailmonkey');
  $variables['styles_url'] = url('admin/config/services/emailmonkey/styles');
  $variables['view_modes_url'] = url('admin/structure/ds/view_modes');

  $variables['screenshot_permissions'] = url("{$module_path}/images/help-permissions.png");
  $variables['screenshot_module_configuration_url'] = url("{$module_path}/images/help-module-configuration.png");
  $variables['screenshot_styles_configuration_url'] = url("{$module_path}/images/help-styles-configuration.png");
  $variables['screenshot_view_mode_configuration_url'] = url("{$module_path}/images/help-view-mode-configuration.png");
  $variables['screenshot_edit_node_url'] = url("{$module_path}/images/help-edit-node.png");
}
