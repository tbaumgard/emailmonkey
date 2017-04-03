<?php
/**
 * @file
 * File for "emailmonkey_preview" template.
 *
 * Available variables:
 *
 * - $node: The associated node.
 * - $error: Whether or not an error occurred.
 * - $error_message: Error message if an error occurred.
 * - $raw_preview_url: The URL for the raw preview if an error didn't occur.
 *
 * @ingroup templates
 */
?>

<?php if (!$error): ?>
  <p><?php print t('You are viewing the preview for %title.', array('%title' => $node->title)); ?></p>
  <iframe class="emailmonkey-preview-raw" src="<?php print check_plain($raw_preview_url); ?>"></iframe>
<?php else: ?>
  <p><?php print $error_message; ?></p>
<?php endif; ?>
