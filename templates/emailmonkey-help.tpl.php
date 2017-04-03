<?php
/**
 * @file
 * File for "emailmonkey_help" template.
 *
 * Available variables:
 *
 * - $permissions_url: The URL to the permissions configuration page.
 * - $content_types_url: The URL to the content types configuration page.
 * - $configuration_url: The URL to the module configuration page.
 * - $styles_url: The URL to the styles configuration page.
 * - $view_modes_url: The URL to the Display Suite view modes page.
 * - $screenshot_permissions: The URL to the permissions screenshot.
 * - $screenshot_module_configuration_url: The URL to the module configuration screenshot.
 * - $screenshot_styles_configuration_url: The URL to the styles configuration screenshot.
 * - $screenshot_view_mode_configuration_url: The URL to the view mode configuration screenshot.
 * - $screenshot_edit_node_url: The URL to the node editing page screenshot.
 *
 * @ingroup templates
 */
?>

<h3><?php print t('Table of Contents'); ?></h3>

<nav>
  <ul>
    <li><a href="#emailmonkey-introduction"><?php print t('Introduction'); ?></a></li>
    <li>
      <a href="#emailmonkey-configuration"><?php print t('Configuration'); ?></a>
      <ul>
        <li><a href="#emailmonkey-configuration-content_type_layouts"><?php print t('Content Type Layouts'); ?></a></li>
      </ul>
    </li>
    <li><a href="#emailmonkey-using_emailmonkey"><?php print t('Using EmailMonkey'); ?></a></li>
    <li><a href="#emailmonkey-css_guide"><?php print t('CSS Guide'); ?></a></li>
    <li><a href="#emailmonkey-resources"><?php print t('Resources'); ?></a></li>
  </ul>
</nav>

<h3 id="emailmonkey-introduction"><?php print t('Introduction'); ?></h3>

<p><?php print t('EmailMonkey is a module that integrates with the MailChimp API and the Display Suite module to make it easy to create custom email templates and to preview, export, test, and send node content as MailChimp campaigns. Everything is configured from Drupal\'s web interface, and the module includes a flexible, table-based Display Suite layout. The only development required is to create CSS style sheets for the templates.'); ?></p>

<p><?php print t('Using Display Suite view modes, EmailMonkey supports multiple email templates per content type. Also, the view mode used to export a node\'s content to MailChimp can be set independently of the view mode used to display its content on the site.'); ?></p>

<p><?php print t('EmailMonkey supports specifying as many styles—in the form of CSS style sheets—as necessary per view mode, which makes it easy to reuse styles among multiple email templates and multiple content types. It also offers granular permissions to support complex workflows.'); ?></p>

<p><?php print t('EmailMonkey also handles preprocessing tasks. It automatically rewrites all URL paths to make them absolute, inlines CSS, and minifies HTML before exporting to MailChimp.'); ?></p>

<h3 id="emailmonkey-configuration"><?php print t('Configuration'); ?></h3>

<p><?php print t('Configuration of the module can be done at <a href="@configuration_url">Configuration > Web Services > EmailMonkey</a>. First, enter an API key from MailChimp and then select the content types for which EmailMonkey functionality should be enabled. <a href="@screenshot_module_configuration_url">View an example configuration</a>.', array('@configuration_url' => $configuration_url, '@screenshot_module_configuration_url' => $screenshot_module_configuration_url)); ?></p>

<p><?php print t('Once that is done, the email styles—in the form of CSS style sheets—to make available for view modes can be configured at <a href="@styles_url">Configuration > Web Services > EmailMonkey > Styles</a>. To make an email style available, simply give it a descriptive name and set the path to its style sheet relative to the Drupal root. This page is also where styles can be deleted. <a href="@screenshot_styles_configuration_url">View an example configuration</a>.', array('@styles_url' => $styles_url, '@screenshot_styles_configuration_url' => $screenshot_styles_configuration_url)); ?></p>

<p><?php print t('Finally, the <a href="@permissions_url">permissions for EmailMonkey</a> need to be configured. The permissions allow both simple and complex workflows to be created, e.g., allowing content creators to create and export email campaigns but allowing only content editors the ability to send campaigns. <a href="@screenshot_permissions">View an example configuration</a>.', array('@permissions_url' => $permissions_url, '@screenshot_permissions' => $screenshot_permissions)); ?></p>

<h4 id="emailmonkey-configuration-content_type_layouts"><?php print t('Content Type Layouts'); ?></h4>

<p><?php print t('First, make sure that EmailMonkey functionality is enabled for the content type on the <a href="@configuration_url">module configuration page</a>. Then, go to the <a href="@content_types_url">content type\'s <em>Manage display</em> page</a> and click the view mode to use. The view mode needs to be enabled under the <em>Custom display settings</em> vertical tab in order for it to show up. Use Display Suite to <a href="@view_modes_url">create a new view mode</a>, e.g., <em>Monthly Newsletter</em>, if necessary.', array('@configuration_url' => $configuration_url, '@content_types_url' => $content_types_url, '@view_modes_url' => $view_modes_url)); ?></p>

<p><?php print t('The layout can be customized as desired, but make sure to set the Display Suite layout to <em>EmailMonkey: Email Template</em> and to select the styles in the EmailMonkey group that should be used. Keep in mind that this page is used to configure all view modes for the content type, so always be sure that the correct one is selected. <a href="@screenshot_view_mode_configuration_url">View an example configuration</a>.', array('@screenshot_view_mode_configuration_url' => $screenshot_view_mode_configuration_url)); ?></p>

<p><?php print t('<b>Tip:</b> The <em>Custom fields</em> vertical tab is useful for adding additional content to the layout, e.g., blocks, custom code, dynamic fields, etc. This can be practically anything, even content from other modules such as the Views module.'); ?></p>

<h3 id="emailmonkey-using_emailmonkey"><?php print t('Using EmailMonkey'); ?></h3>

<p><?php print t('To use EmailMonkey for a node, the view mode that EmailMonkey should use when previewing and exporting it has to be selected. This can be done by setting the <em>View mode</em> field available in the <em>EmailMonkey</em> vertical tab on the node editing page. <a href="@screenshot_edit_node_url">View an example</a>.', array('@screenshot_edit_node_url' => $screenshot_edit_node_url)); ?></p>

<p><?php print t('Once that\'s done, EmailMonkey functionality for the node is accessed through the <em>EmailMonkey</em> page tab for the node. The page shows an inline preview of the content and includes a few links: a link to go back to the node page; a link to view a raw preview of the node, i.e., a preview of exactly what gets exported to MailChimp; a link to export the content to MailChimp as a campaign; a link to delete the MailChimp campaign or just the Drupal site\'s reference to it; a link to update the campaign in case the content, layout, or styles have changed since the last update or export; a link to test the campaign; and a link to send the campaign. The options that are displayed at any given time depend on user permissions, e.g., whether the current user is allowed to export or send content to MailChimp, and the state of the node, e.g., the export link isn\'t displayed if the content has already been exported to MailChimp. '); ?></p>

<p><?php print t('<b>Note about testing:</b> the MailChimp API only allows a limited amount of tests per campaign and a limited amount of tests per account per day, so use that option only when necessary, e.g., the campaign is ready to be sent. To test during development, it\'s better to use the <em>Send using Drupal</em> option. It\'s also a good idea to test an email template at least once with a service such as <a href="https://litmus.com/">Litmus</a> or <a href="https://www.emailonacid.com/">Email on Acid</a> to make sure it displays as intended in various email clients.'); ?></p>

<h3 id="emailmonkey-css_guide"><?php print t('CSS Guide'); ?></h3>

<p><?php print t('The <i>EmailMonkey: Email Template</i> Display Suite layout uses a number of nested tables in order to make it flexible enough to work for a wide range of email templates and a wide variety of email clients. Additionally, a field\'s content will be wrapped in whatever element is chosen in the <i>Custom wrappers</i> vertical tab on a <a href="@content_types_url">content type\'s <i>Manage display</i> page</a>. Regions without content won\'t be included in the output.', array('@content_types_url' => $content_types_url)); ?></p>

<p><?php print t('The following CSS code is a comprehensive skeleton to start from when creating style sheets for email templates. Except where noted, each block indicates a new level of nesting.'); ?></p>

<p><pre><code>
/* Each of these is a new nesting level. */
#emailmonkey-body-outer_table { }
#emailmonkey-body-outer_row { }
#emailmonkey-body-outer_cell { }

/* Each of these is a new nesting level. */
#emailmonkey-body-inner_table { }
.emailmonkey-body-inner_row { }
.emailmonkey-body-inner_cell { }

#emailmonkey-body-inner_row-header { }
#emailmonkey-body-inner_row-hero { }
#emailmonkey-body-inner_row-main_content { }
#emailmonkey-body-inner_row-postscript { }
#emailmonkey-body-inner_row-footer { }
#emailmonkey-body-inner_row-fine_print { }

#emailmonkey-body-inner_cell-header { }
#emailmonkey-body-inner_cell-hero { }
#emailmonkey-body-inner_cell-main_content { }
#emailmonkey-body-inner_cell-postscript { }
#emailmonkey-body-inner_cell-footer { }
#emailmonkey-body-inner_cell-fine_print { }

.emailmonkey-region-outer_table { }
#emailmonkey-header-outer_table { }
#emailmonkey-hero-outer_table { }
#emailmonkey-main_content-outer_table { }
#emailmonkey-postscript-outer_table { }
#emailmonkey-footer-outer_table { }
#emailmonkey-fine_print-outer_table { }

.emailmonkey-region-outer_row { }
#emailmonkey-header-outer_row { }
#emailmonkey-hero-outer_row { }
#emailmonkey-main_content-outer_row { }
#emailmonkey-postscript-outer_row { }
#emailmonkey-footer-outer_row { }
#emailmonkey-fine_print-outer_row { }

.emailmonkey-region-outer_cell { }
#emailmonkey-header-outer_cell { }
#emailmonkey-hero-outer_cell { }
#emailmonkey-main_content-outer_cell { }
#emailmonkey-postscript-outer_cell { }
#emailmonkey-footer-outer_cell { }
#emailmonkey-fine_print-outer_cell { }

.emailmonkey-region-table { }
#emailmonkey-header-table { }
#emailmonkey-hero-table { }
#emailmonkey-main_content-table { }
#emailmonkey-postscript-table { }
#emailmonkey-footer-table { }
#emailmonkey-fine_print-table { }

.emailmonkey-region-row { }
#emailmonkey-header-row { }
#emailmonkey-hero-row { }
#emailmonkey-main_content-row { }
#emailmonkey-postscript-row { }
#emailmonkey-footer-row { }
#emailmonkey-fine_print-row { }

/* These are at the same level as the -left, -center, and -right declarations below.  */
.emailmonkey-header-cell { }
.emailmonkey-hero-cell { }
.emailmonkey-main_content-cell { }
.emailmonkey-postscript-cell { }
.emailmonkey-footer-cell { }
.emailmonkey-fine_print-cell { }

.emailmonkey-region-left { }
#emailmonkey-header-left { }
#emailmonkey-hero-left { }
#emailmonkey-main_content-left { }
#emailmonkey-postscript-left { }
#emailmonkey-footer-left { }
#emailmonkey-fine_print-left { }

.emailmonkey-region-center { }
#emailmonkey-header-center { }
#emailmonkey-hero-center { }
#emailmonkey-main_content-center { }
#emailmonkey-postscript-center { }
#emailmonkey-footer-center { }
#emailmonkey-fine_print-center { }

.emailmonkey-region-right { }
#emailmonkey-header-right { }
#emailmonkey-hero-right { }
#emailmonkey-main_content-right { }
#emailmonkey-postscript-right { }
#emailmonkey-footer-right { }
#emailmonkey-fine_print-right { }
</code></pre></p>

<p><?php print t('Fields and other content will also contain their own wrappers, classes, and IDs. To determine those, it\'s probably best to use your browser\'s web page inspection features, if available.'); ?></p>

<h3 id="emailmonkey-resources"><?php print t('Resources'); ?></h3>

<ul>
  <li><a href="https://www.campaignmonitor.com/css/">Campaign Monitor's <i>The Ultimate Guide to CSS</i> for HTML email</a></li>
  <li><a href="http://emailclientmarketshare.com">Litmus's <i>Email Client Market Share</i></a></li>
  <li><a href="http://www.htmlemailgallery.com"><i>HTML Email Gallery</i></a></li>
</ul>
