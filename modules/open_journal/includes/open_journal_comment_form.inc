<?php

function open_journal_comment_form($form, $form_state, $journal, $discussion, $data=NULL) {
  
  global $user;
  
  $form['#attached']['js'][] = drupal_get_path('module', 'open_journal').'/js/open_journal_compact.js';
  
  $submit_text = t('Add this comment');
  if (open_journal_get_field($data, 'cid')) {
    $form['cid'] = array(
      '#type' => 'hidden',
      '#value' => $data->cid,
      '#access' => FALSE,
    );
    
    $submit_text = t('Save changes');
  }
  
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#required' => TRUE,
    '#default_value' => open_journal_get_field($data, 'message'),
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
    '#value' => $submit_text,
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );
  
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

  $form['did'] = array(
    '#type' => 'hidden',
    '#value' => $discussion->did,
    '#access' => FALSE,
  );

  
  return $form;
}


function open_journal_comment_form_submit($form, &$form_state) {
  
  $data = (object) $form_state['values'];
  open_journal_comment_save($data);
  
  $is_new = (isset($data->cid) && $data->cid)? FALSE: TRUE;
  
  if ($is_new) {
    drupal_set_message(t('You has been reply new comment.'));
  }
  else {
    drupal_set_message(t('This comment has been updated.'));
  }
  
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$data->jid.'/discussion/'.$data->did.'/comment/'.$data->cid);
  
}


function open_journal_comment_delete_form($form, &$form_state, $journal, $discussion, $data) {
  
  $form['jid'] = array(
    '#type' => 'hidden',
    '#value' => $journal->jid,
    '#access' => FALSE
  );
  $form['did'] = array(
    '#type' => 'hidden',
    '#value' => $data->did,
    '#access' => FALSE
  );
  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $data->cid,
    '#access' => FALSE,
  );
  
  $title = t('Are you sure you want to delete the comment?');
  
  $form['title'] = array(
    '#markup' => '<h2>'.$title.'</h2>',
  );
  
  return confirm_form($form, $title, OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$data->did.'/comment/'.$data->cid);
}

function open_journal_comment_delete_form_submit($form, &$form_state) {
  open_journal_discussion_remove($form_state['values']['cid']);
  drupal_set_message(t('The comment has been deleted.'));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$form_state['values']['jid'].'/discussion/'.$form_state['values']['did']);
}