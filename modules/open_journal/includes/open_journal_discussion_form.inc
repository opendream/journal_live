<?php 
function open_journal_discussion_form($form, &$form_state, $journal, $data=NULL) {

  global $user; 

  $form['head_title'] = array(
    '#markup' => '<h2>'.t('Add new topic').'</h2>',
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Detail'),
  );

  $form['fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('File'),
    '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
    '#upload_validators' => array(
      'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FILE_TYPE),
      //'file_validate_size' => array(2 * 1048576),\
    ),
    '#upload_location' => "public://journal/",
  );

  $form['contributor_only'] = array(
    '#title' => t('Contributor only'),
    '#type' => 'checkbox',
    '#description' => t('If checked this box, only contributor can access this discussion.'),
  );

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['main']['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );

  // Hidden field
  if (open_journal_get_field($data, 'did')) {
    $form['did'] = array(
      '#type' => 'hidden',
      '#value' => $data->did,
      '#access' => FALSE,
    );
  }
  else {
    $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $user->uid,
      '#access' => FALSE,
    );

    $form['jid'] = array(
      '#type' => 'hidden',
      '#value' => $journal->jid,
      '#access' => FALSE,
    );
  }



  return $form;
}

function open_journal_discussion_form_submit($form, &$form_state) {
  
  $data = (object) $form_state['values'];
  open_journal_discussion_save($data);

}