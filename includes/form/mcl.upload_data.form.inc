<?php
/**
 * @file
*/
/**
 * MCL Uploading Data form.
*
* @param array $form
* @param array $form_state
* @param string $op
* @param integer $job_id
*/
function mcl_upload_data_form($form, &$form_state) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Upload Data.
  $form['upload_data'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Uploading Data',
  );

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));
  if (!$mcl_user) {
    $form['upload_data']['#collapsed'] = FALSE;
    $form['upload_data']['err'] = array(
      '#markup' => "You don't have the permission to upload data. Please ask site manager to get the permission.",
    );
  }
  else {

    // Upload Data.
    $desc = "MCL data uploading phases.<br /><br />
    <p>Phase 1 : checks syntax errors.<br />
       It checks syntax errors such as missing columns, missing data on required columns, miss-spelled
       column name and so on.</p>
    <p>Phase 2 : checks data errors.<br />
       The data errors are the errors on data in database. For instance, genus and species are
       listed in stock data template. MCL checks if organism_id for these genus and species
       exists in organism table. If not found, it throws an error and it adds an appropriate error
       message to the error log file.</p>
    <p>Phase 3 : uploads data templates.<br />
       After checking errors, it finally starts uploading data template into chado schema.</p>
    <p>During the uploading phases, MCL creates several different types of log files and stores in the
    job directory. MCL seizes the process if it finds an error and ask a user to fix the data. The
    user goes though the all log files to find the errors and fix them. After fixing the error,
    the user come back to the \"Upload Job\" page to re-upload the fixed data file. Then, perform
    re-run job.</p>
    ";
    $form['upload_data']['desc'] = array(
      '#markup' => $desc,
    );

    // Adds the job table.
    $form['job_list'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => FALSE,
      '#collapsible'  => TRUE,
      '#title'        => 'Uploading Job List',
    );
    $form['job_list']['job_table'] = array(
      '#markup' => _mcl_get_uploading_job_table(),
    );

    // Adds the add form.
    _mcl_get_add_job_form($form, $mcl_user);
  }

  $form['home_link'] = array(
    '#markup' => "<div style=''><em>Returns to <a href='/mcl'>MCL home</a></em></div>",
  );

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-upload-data-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns a list of all uploading jobs in a table.
 *
 * @return string
 */
function _mcl_get_uploading_job_table() {
  global $user;

  // Sets the headers.
  $headers = array(
    array('data' => 'Job ID', 'style' => 'width:50px'),
    array('data' => 'Status', 'style' => 'width:60px'),
    array('data' => 'Name', 'style' => ''),
    array('data' => 'Submit Date', 'style' => 'width:120px'),
    array('data' => 'Details', 'style' => 'width:40px'),
    array('data' => 'Action', 'style' => 'width:40px'),
  );

  // Gets all jobs.
  $mcl_jobs = MCL_JOB::getJobsByUser($user->uid);
  $rows = array();
  foreach ($mcl_jobs as $mcl_job) {
    $job_id = $mcl_job->getJobID();

    // Creates 'Delete' link.
    $confirm_attr = array(
      'attributes' => array(
        'id' => 'delete_job_' . $job_id,
        'class' => array('use-ajax', 'mcl-confirm'),
      )
    );
    $delete_link = l('delete', "mcl/delete_job/$job_id", $confirm_attr);

    // Adds the row.
    $row = array(
      $job_id,
      $mcl_job->getStatusLabel(),
      $mcl_job->getName(),
      $mcl_job->getSubmitDate(),
      l('view', "/mcl/job_view/$job_id"),
      $delete_link,
    );
    $rows []= $row;
  }

  // Gets the upload_job information.
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => t("You don't have any uploading job."),
    'attributes'  => array(),
  );
  return theme('table', $table_vars);
}

/**
 * Adds form elements for adding a job.
 *
 * @param array $form
 * @param MCL_USER $mcl_user
 */
function _mcl_get_add_job_form(&$form, MCL_USER $mcl_user) {
  global $user;

  // Adds a add_upload_job form.
  $form['add_upload_job'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Add Upload Job',
  );
  $form['add_upload_job']['name'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Job Name',
    '#description'  => "Please specify the name of a job.",
    '#required'     => TRUE,
  );
  $form['add_upload_job']['upload_file'] = array(
    '#type'               => 'managed_file',
    '#title'              => 'Data template file',
    '#description'        => "Please provide a data template file.",
    '#upload_location'    => $mcl_user->getUserDirURL(),
    '#required'           => TRUE,
    '#progress_indicator' => 'bar',
    '#upload_validators'  => array(
      'file_validate_extensions' => array('xls xlsx zip csv'),
    ),
  );
  $form['add_upload_job']['add_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'add_btn',
    '#value'  => 'Add a uploading job',
  );
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_validate($form, &$form_state) {
  global $user;

  // Checks job name for duplication.
  $name = trim($form_state['values']['add_upload_job']['name']);
  $mcl_job = MCL_JOB::byKey(array('name' => $name, 'user_id' => $user->uid));
  if ($mcl_job) {
    form_set_error('add_upload_job][name', t("The job name '$name' exists. Please change the name of the job."));
  }
}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_submit($form, &$form_state) {
  global $user;

  // Gets MCL_USER.
  $mcl_user   = MCL_USER::byKey(array('user_id' => $user->uid));
  $user_name  = $mcl_user->getName();
  $user_dir   = $mcl_user->getUserDir();

  // Gets the job info.
  $job_name     = trim($form_state['values']['add_upload_job']['name']);
  $drupal_file  = file_load($form_state['values']['add_upload_job']['upload_file']);

  // Create the user temp directory.
  $user_tmp_dir = "$user_dir/tmp";
  mcl_create_dir($user_tmp_dir);

  // Moves the file to the user temp directory.
  $dest_filepath    = "$user_tmp_dir/" . $drupal_file->filename;
  $target_filepath  = drupal_realpath($drupal_file->uri);
  $cmd              = "mv $target_filepath $dest_filepath";
  exec($cmd);

  // Removes the uploaded file.
  //file_delete($drupal_file);
  db_delete('file_managed')
    ->condition('fid', $drupal_file->fid, '=')
    ->execute();

  // Submits a job.
  $cmd = "drush mcl-upload-data $user_name $dest_filepath " .
    " --working_dir=" . $mcl_user->getUserDir() .
    " --job_name=\"$job_name\"  --move_input &";
  exec($cmd);
}