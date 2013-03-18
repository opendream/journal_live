<?php 
function open_journal_discussion_form($form, &$form_state, $journal, $data=NULL) {

  global $user; 
  
  $form['#attached']['css']['wmd'] = drupal_get_path('module', 'open_journal').'/wmd/wmd.css';
  $form['#attached']['js']['wmd'] = drupal_get_path('module', 'open_journal').'/wmd/wmd.js';
  $form['#attached']['js']['open_journal_editor'] = drupal_get_path('module', 'open_journal').'/js/open_journal_editor.js';
  
  
  if ($data && $data->did) {
    $form['head_title'] = array(
      '#markup' => '<h2>'.t('Edit topic').'</h2>',
    );

    $submit_text = t('Save changes');
  }
  else {
    $form['head_title'] = array(
      '#markup' => '<h2>'.t('Add new topic').'</h2>',
    );
    $submit_text = t('Start new discussion');
  }
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => open_journal_get_field($data, 'title')
  );
  
  $form['description'] = array(
    '#type' => 'textarea',
    //'#title' => t('Details'),
    '#default_value' => open_journal_get_field($data, 'description'),
    '#attributes' => array('id' => 'wmd-input-editor', 'class' => array('wmd-input')),
    '#prefix' => '<div id="wmd-toolbar-editor" class="wmd-toolbar">'.'</div>',
    '#default_value' => open_journal_get_field($data, 'description')
  );

  if($journal->status < count(unserialize(OPEN_JOUNAL_STATUS))-1) {
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
  }
  else {
    $form['fid'] = array(
    '#type' => 'managed_file',
    '#title' => t('File'),
    '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FINAL_STATUS_FILE_TYPE)),
    '#upload_validators' => array(
      'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FINAL_STATUS_FILE_TYPE),
      //'file_validate_size' => array(2 * 1048576),
    ),
    '#upload_location' => "public://journal/",
    '#default_value' => open_journal_get_field($data, 'fid'),
  );
  }

  
      
  $form['contributor_only'] = array(
    '#title' => t('Contributor only'),
    '#type' => 'checkbox',
    '#description' => t('If checked this box, only contributor can access this discussion.'),
    '#default_value' => open_journal_get_field($data, 'contributor_only', 1),
    '#access' => !open_journal_get_field($data, 'permanent') && user_access('access contributor process')
  );

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  $form['main']['actions']['cancel'] = array(
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

  // Hidden field
  if (open_journal_get_field($data, 'did')) {
    $form['did'] = array(
      '#type' => 'hidden',
      '#value' => $data->did,
      '#access' => FALSE,
    );
  }



  return $form;
}

function open_journal_discussion_form_submit($form, &$form_state) {
  
  $data = (object) $form_state['values'];
  open_journal_discussion_save($data);
  
  $is_new = (isset($data->did) && $data->did)? FALSE: TRUE;
  
  if ($is_new) {
    drupal_set_message(t('You has been start new disscussion completed.'));
  }
  else {
    drupal_set_message(t('This disscussion has been updated.'));
  }
  
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$data->jid.'/discussion/'.$data->did);
  
}

function open_journal_discussion_delete_form($form, &$form_state, $journal, $data) {
  
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
  
  $title = t('Are you sure you want to delete the discussion %title?', array('%title' => $data->title));
  
  $form['title'] = array(
    '#markup' => '<h2>'.$title.'</h2>',
  );
  
  return confirm_form($form, $title, OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$data->did);
}

function open_journal_discussion_delete_form_submit($form, &$form_state) {
  open_journal_discussion_remove($form_state['values']['did']);
  drupal_set_message(t('The discussion has been deleted.'));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$form_state['values']['jid'].'/discussion');
}
