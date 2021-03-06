<?php
/**
 * @file
 * This file contains the functions used for administration of the template.
 *
 * @ingroup mcl_admin
 */
/**
 * Admin page.
 */
function mcl_admin_page() {

  // Set the breadcrumb.
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  drupal_set_breadcrumb($breadcrumb);

  // Page contents.
  $content = '<b>MCL admin main page</b><br /><br />';

  // Check MCL directories.
  $dirs = array();
  $working_dir = mcl_get_config_setting('mcl_working_dir');
  $dirs['working_dir']  = $working_dir;
  $dirs['user_dir']     = "$working_dir/user";
  $content .= '<b>Checking MCL directories</b><br /><br />';
  foreach ($dirs as $name => $dir) {
    $content .= "<b>$name :</b> $dir";
    if (file_exists($dir)) {
      $content .= " ... ok<br />";
    }
    else {
      $content .= '<br />The directory does not exist, trying to create a directory<br />';
      if(!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
        $content .= 'Fail to create a directory<br /><br />';
        watchdog('mcl', $message, array(), WATCHDOG_ERROR);
      }
      else {
        $content .= 'The directory has been created<br /><br />';
      }
    }
  }
  return $content;
}
