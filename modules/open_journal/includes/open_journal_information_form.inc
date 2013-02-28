<?php

function open_journal_information_form($form, &$form_state , $data=NULL) {

  global $user;

  $mpath = drupal_get_path('module', 'open_journal');
  drupal_add_js($mpath.'/js/open_journal_multi_textfield.js');

  // Main form
  $form['main'] = array('#type' => 'container');

  $form['main']['main_title'] = array('#markup' => '<h2>'.t('Information').'</h2>');

  $form['main']['dc_title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => open_journal_get_field($data, 'dc_title'),
  );

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['main']['description_sub'] = array(
      '#title' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_LABEL,
      '#type' => 'textarea',
      '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
      '#required' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_REQUIRED,
      '#default_value' => open_journal_get_field($data, 'description_sub'),
    );
  }

  $form['main']['description_en'] = array(
    '#title' => t('Abstract'),
    '#type' => 'textarea',
    '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
    '#required' => OPEN_JOURNAL_ABSTRACT_REQUIRED,
    '#default_value' => open_journal_get_field($data, 'description_en'),
  );

  if(isset($data->jid)) {
    // TODO: create discussion
    $form['main']['file_link'] = array(
      '#markup' => 'Latest file : '.l('filename.doc', '').'<div class="description">'.t('When you create discussion or comment with upload file, The latest file will be update.').'</div>'
    );
  }
  else {
    $form['main']['file'] = array(
      '#type' => 'managed_file',
      '#title' => t('File'),
      '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
      '#upload_validators' => array(
        'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FILE_TYPE),
        //'file_validate_size' => array(2 * 1048576),\
      ),
      '#upload_location' => "public://journal/",
      '#required' => TRUE,
    );
  }

  $form['main']['guideline'] = array(
    '#title' => t('I read submit journal guideline completely.').' '.l(t('Download guideline'), ''),
    '#type' => 'checkbox',
    '#required' => TRUE,
  );

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['main']['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );


  // Sidebar form

  $form['data'] = array('#type' => 'container');
  $form['data']['data_title'] = array('#markup' => '<h3>'.t('Data').'</h3>');

  $form['data']['group'] = array('#type' => 'fieldset');

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['subject_sub'] = open_journal_multi_textfield(OPEN_JOURNAL_SUB_LANG_KEYWORDS_LABEL,open_journal_get_field($data, 'subject_sub'));
  }
  $form['data']['group']['subject_en'] = open_journal_multi_textfield('Keywords',open_journal_get_field($data, 'subject_en'));

  $form['data']['group']['dc_creator'] = open_journal_multi_textfield('Creators' ,open_journal_get_field($data, 'dc_creator'));
  $form['data']['group']['dc_publisher'] = open_journal_multi_textfield('Publishers' ,open_journal_get_field($data, 'dc_publisher'));
  $form['data']['group']['dc_contributor'] = open_journal_multi_textfield('Contributors', open_journal_get_field($data, 'dc_contributor'));

  // Setting date to display form
  if(isset($data->jid)) {
    $year = date('Y',$data->dc_date);
    $month = date('m',$data->dc_date);
    $day = date('d',$data->dc_date); 
  }
  else {
    $year = date('Y');
    $month = date('m');
    $day = date('d');
  }
   
  // Group date 

  $form['data']['group']['date'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE
  );
  $form['data']['group']['date']['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(date('Y')-10,date('Y')),
    '#default_value' => $year,
    '#suffix' => '/',
  );
  $form['data']['group']['date']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,12),
    '#default_value' => $month,
    '#suffix' => '/',

  );
  $form['data']['group']['date']['day'] = array(
    '#title' => t('Day'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,30),
    '#default_value' => $day,
  );

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['dc_language'] = array(
      '#title' => 'Language',
      '#type' => 'select',
      '#options' => array(
        OPEN_JOURNAL_SUB_LANG_CODE => t(OPEN_JOURNAL_SUB_LANG_TEXT),
        'en' => 'English',
      ),
      '#default_value' => open_journal_get_field($data, 'dc_language')
    );
  }
  else {
    $form['data']['group']['dc_language'] = array(
      '#type' => 'hidden',
      '#value' => 'en',
      '#access' => FALSE,
    );
  }

  $form['data']['group']['dc_identifier'] = array(
    '#title' => 'Final URL',
    '#type' => 'textfield',
    '#description' => t('For librarian only'),
    '#access' => user_access('update final url field'),
    '#default_value' => open_journal_get_field($data, 'dc_identifier'),
  );

  // Hidden field

  $form['data']['group']['dc_type'] = array(
    '#type' => 'hidden',
    '#value' => 'Journal',
    '#access' => FALSE,
  );

  $form['data']['group']['dc_format'] = array(
    '#type' => 'hidden',
    '#value' => 'application/doc',
    '#access' => FALSE,
  );

  $form['data']['group']['dc_coverage'] = array(
    '#type' => 'hidden',
    '#value' => '',
    '#access' => FALSE,
  );


  $form['data']['group']['dc_right'] = array(
    '#type' => 'hidden',
    '#value' => '',
    '#access' => FALSE,
  );

  if (open_journal_get_field($data, 'jid')) {
    $form['jid'] = array(
      '#type' => 'hidden',
      '#value' => $data->jid,
      '#access' => FALSE,
    );
  }
  else {
    $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $user->uid,
      '#access' => FALSE,
    );
  }

  
  

  return $form;
}

function open_journal_get_date_option($start_num , $end_num) {
  $option = array();
  for($i = $start_num; $i <= $end_num; $i++) {
    if($i < 10) {
      $i = '0'.$i;
    }
    $option[$i] = $i;
  }
  return $option;
}

function open_journal_multi_textfield($title='' , $values = FALSE) {

  $multi_textfield = array(
    '#type' => 'fieldset',
    '#title' => t($title),
    '#tree' => TRUE,
  );

  for ($i = 0; $i < OPEN_JOURNAL_MAX_MULTI_TEXTFIELD; $i++) {
    if(!isset($values[$i])) {
      $values[$i] = '';
    }
    $multi_textfield[] = array(
      '#type' => 'textfield',
      '#attributes' => array('class' => array('multi-textfield', 'multi-textfield-'.$i)),
      '#default_value' => $values[$i],
    );
  }
  $multi_textfield[] = array('#markup' => '<button class="add-multi-textfield">'.t('Add another').'</button>');

  return $multi_textfield;
}

function open_journal_information_form_validate($form, &$form_state) {
  
  $values = $form_state['values'];
  
  if (mb_strlen($values['description_en']) > OPEN_JOURNAL_ABSTRACT_MAX_LENGTH) {
    form_set_error('description_en', t('Abstract field must be later than %d character.', array(
      '%d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH
    )));
  }

  if (OPEN_JOURNAL_SUB_LANG_ENABLE && mb_strlen($values['description_sub']) > OPEN_JOURNAL_ABSTRACT_MAX_LENGTH) {
    form_set_error('description_sub', t('!s field must be later than %d character.', array(
      '!s' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_LABEL,
      '%d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH
    )));


  }

}

function open_journal_information_form_submit($form, &$form_state) {
  
  $data = (object) $form_state['values'];
  open_journal_journal_save($data);

}