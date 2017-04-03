<?php
/**
 * @file
 * File for "emailmonkey_delete" template.
 *
 * Available variables:
 *
 * - $node: The associated node.
 * - $error: Whether or not an error occurred.
 * - $error_message: Error message if an error occurred.
 * - $form: Deletion form if an error didn't occur.
 *
 * @ingroup templates
 */
?>

<?php if (!$error): ?>
  <?php print render($form); ?>
<?php else: ?>
  <p><?php print $error_message; ?></p>
<?php endif; ?>
