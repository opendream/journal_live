<?php

function open_journal_comment_form($form, &$form_state, $discussion, $data=NULL) {
  
  global $user;
  
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    
  );
  
  $form['fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('File'),
    '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
    '#upload_validators' => array(
      'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FILE_TYPE),
      //'file_validate_size' => array(2 * 1048576),
    ),
    '#upload_location' => "public://journal/",
    '#default_value' => open_journal_get_field($data, 'fid'),
  );
  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );
  
  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $user->uid,
    '#access' => FALSE,
  );

  $form['did'] = array(
    '#type' => 'hidden',
    '#value' => $discussion->did,
    '#access' => FALSE,
  );

  // Hidden field
  if (open_journal_get_field($data, 'cid')) {
    $form['cid'] = array(
      '#type' => 'hidden',
      '#value' => $data->cid,
      '#access' => FALSE,
    );
  }
  
  return $form;
}