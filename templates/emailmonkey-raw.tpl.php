<?php
/**
 * @file
 * File for "emailmonkey_raw" template.
 *
 * Available variables:
 *
 * - $node: The associated node.
 * - $error: Whether or not an error occurred.
 * - $error_message: Error message if an error occurred.
 * - $rendered_node: Rendered node prepared for sending in an email if an error
 *   didn't occur.
 * - $css: The CSS styles as text to include if an error didn't occur.
 *
 * @ingroup templates
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <!--[if !mso]><!-- -->
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <!--<![endif]-->
    <title><?php print check_plain($node->title); ?></title>
    <style type="text/css">
     <?php if (!$error): ?>
     <?php print $css; ?>
     <?php endif; ?>
    </style>
  </head>
  <body>
    <?php if (!$error): ?>
      <?php print $rendered_node; ?>
    <?php else: ?>
      <p><?php print $error_message; ?></p>
    <?php endif; ?>
  </body>
</html>
