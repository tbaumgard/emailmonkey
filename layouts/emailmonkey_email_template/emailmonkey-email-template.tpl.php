<?php
/**
 * @file
 * Email template for Display Suite.
 *
 * Available variables:
 *
 * Layout:
 * - $contextual_links: Renderable array of contextual links.
 * - ${header,hero,main_content,postscript,footer}_{left,center,right}_classes: String of classes that can be used to style this layout.
 * - ${header,hero,main_content,postscript,footer}_{left,center,right}_wrapper: Wrapper surrounding the layout.
 * - ${header,hero,main_content,postscript,footer}_{left,center,right}: Region content.
 */

$rows = array('header', 'hero', 'main_content', 'postscript', 'footer', 'fine_print');

?>

<center>
  <table id="emailmonkey-body-outer_table" class="emailmonkey" align="center" border="0" cellpadding="0" cellspacing="0" height="100%" width="100%" <?php print $layout_attributes; ?>>
    <tbody>
      <tr id="emailmonkey-body-outer_row">
        <td id="emailmonkey-body-outer_cell" align="center" border="0" valign="top">
          <table id="emailmonkey-body-inner_table" align="center" border="0" cellpadding="0" cellspacing="0" width="100%">
            <tbody>
              <?php foreach ($rows as $row): ?>
                <?php $left_region = ${$row."_left"}; ?>
                <?php $center_region = ${$row."_center"}; ?>
                <?php $right_region = ${$row."_right"}; ?>
                <?php $left_wrapper = ${$row."_left_wrapper"}; ?>
                <?php $center_wrapper = ${$row."_center_wrapper"}; ?>
                <?php $right_wrapper = ${$row."_right_wrapper"}; ?>
                <?php $left_classes = ${$row."_left_classes"}; ?>
                <?php $center_classes = ${$row."_center_classes"}; ?>
                <?php $right_classes = ${$row."_right_classes"}; ?>

                <?php if (!empty($left_region) || !empty($center_region) || !empty($right_region)): ?>
                  <tr id="emailmonkey-body-inner_row-<?php print $row; ?>" class="emailmonkey-body-inner_row">
                    <td id="emailmonkey-body-inner_cell-<?php print $row; ?>" class="emailmonkey-body-inner_cell" align="center" valign="top">
                      <table id="emailmonkey-<?php print $row; ?>-outer_table" class="emailmonkey-region-outer_table" align="center" border="0" cellpadding="0" cellspacing="0" width="100%">
                        <tbody>
                          <tr id="emailmonkey-<?php print $row; ?>-outer_row" class="emailmonkey-region-outer_row">
                            <td id="emailmonkey-<?php print $row; ?>-outer_cell" class="emailmonkey-region-outer_cell" align="center" valign="top">
                              <table id="emailmonkey-<?php print $row; ?>-table" class="emailmonkey-region-table" align="center" border="0" cellpadding="0" cellspacing="0">
                                <tbody>
                                  <tr id="emailmonkey-<?php print $row; ?>-row" class="emailmonkey-region-row">
                                    <?php if (!empty($left_region)): ?>
                                      <td id="emailmonkey-<?php print $row; ?>-left" class="emailmonkey-<?php print $row; ?>-cell emailmonkey-region-left" align="center" border="0" valign="top">
                                        <<?php print $left_wrapper; ?> class="<?php $left_classes; ?>">
                                          <?php print $left_region; ?>
                                        </<?php print $left_wrapper; ?>>
                                      </td>
                                    <?php endif; ?>
                                    <?php if (!empty($center_region)): ?>
                                      <td id="emailmonkey-<?php print $row; ?>-center" class="emailmonkey-region-center" align="center" border="0" valign="top">
                                        <<?php print $center_wrapper; ?> class="<?php $center_classes; ?>">
                                          <?php print $center_region; ?>
                                        </<?php print $center_wrapper; ?>>
                                      </td>
                                    <?php endif; ?>
                                    <?php if (!empty($right_region)): ?>
                                      <td id="emailmonkey-<?php print $row; ?>-right" class="emailmonkey-region-right" align="center" border="0" valign="top">
                                        <<?php print $right_wrapper; ?> class="<?php $right_classes; ?>">
                                          <?php print $right_region; ?>
                                        </<?php print $right_wrapper; ?>>
                                      </td>
                                    <?php endif; ?>
                                  </tr>
                                </tbody>
                              </table>
                            </td>
                          </tr>
                        </tbody>
                      </table>
                    </td>
                  </tr>
                <?php endif; ?>
              <?php endforeach; ?>
            </tbody>
          </table>
        </td>
      </tr>
    </tbody>
  </table>
</center>
