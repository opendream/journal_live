<?php

function open_journal_information_form($form, &$form_state) {

  $mpath = drupal_get_path('module', 'open_journal');
  drupal_add_js($mpath.'/js/open_journal_multi_textfield.js');

  // Main form

  $form['main'] = array('#type' => 'container');

  $form['main']['main_title'] = array('#markup' => '<h2>'.t('Information').'</h2>');

  $form['main']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['main']['description_sub'] = array(
      '#title' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_LABEL,
      '#type' => 'textarea',
      '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
      '#required' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_REQUIRED,
    );
  }

  $form['main']['description_en'] = array(
    '#title' => t('Abstract'),
    '#type' => 'textarea',
    '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
    '#required' => OPEN_JOURNAL_ABSTRACT_REQUIRED,
  );

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

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['main']['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );


  // Sidebar form

  $form['data'] = array('#type' => 'container');
  $form['data']['data_title'] = array('#markup' => '<h3>'.t('Data').'</h3>');

  $form['data']['group'] = array('#type' => 'fieldset');

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['subject_sub'] = open_journal_multi_textfield(OPEN_JOURNAL_SUB_LANG_KEYWORDS_LABEL);
  }
  $form['data']['group']['subject_en'] = open_journal_multi_textfield('Keywords');

  $form['data']['group']['dc_creator'] = open_journal_multi_textfield('Creators');
  $form['data']['group']['dc_publisher'] = open_journal_multi_textfield('Publishers');
  $form['data']['group']['dc_contributor'] = open_journal_multi_textfield('Contributors');

  $form['data']['group']['date'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE
  );
  $form['data']['group']['date']['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(date('Y')-10,date('Y')),
    '#default_value' => date('Y'),
    '#suffix' => '/',
  );
  $form['data']['group']['date']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,12),
    '#default_value' => date('m'),
    '#suffix' => '/',

  );
  $form['data']['group']['date']['day'] = array(
    '#title' => t('Day'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,30),
    '#default_value' => date('d'),
  );

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['dc_language'] = array(
      '#title' => 'Language',
      '#type' => 'select',
      '#options' => array(
        OPEN_JOURNAL_SUB_LANG_CODE => t(OPEN_JOURNAL_SUB_LANG_TEXT),
        'en' => 'English',
      ),
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
    '#access' => user_access('update final url field')
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

function open_journal_multi_textfield($title='') {
  $multi_textfield = array(
    '#type' => 'fieldset',
    '#title' => t($title),
    '#tree' => TRUE,
  );

  for ($i = 0; $i < OPEN_JOURNAL_MAX_MULTI_TEXTFIELD; $i++) {
    $multi_textfield[] = array(
      '#type' => 'textfield',
      '#attributes' => array('class' => array('multi-textfield', 'multi-textfield-'.$i))
    );
  }
  $multi_textfield[] = array('#markup' => '<button class="add-multi-textfield">'.t('Add').'</button>');

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
  $item = (object) $form_state['values'];

  $file = file_load($item->file);

  /*
  $insert = db_insert('open_journal_journal')->fields(array(
    'created'         => REQUEST_TIME, 
    'changed'         => REQUEST_TIME, 
    'status_changed'  => REQUEST_TIME, 
    'title'           => $values['title'],
    'description_en'  => $values['description_en'], 
    'description_sub' => $values['description_sub'],
    'subject_en'      => implode(',', $values['subject_en']),
    'subject_sub'     => implode(',', $values['subject_sub']),
    'creator'         => implode(',', $values['creator']), 
    'publisher'       => implode(',', $values['publisher']), 
    'contributor'     => implode(',', $values['contributor']),
    'date'            => '', 
    'language'        => $values['language'], 
    'identifier'      => $values['identifier'],
    'type'            => $values['type'], 
    'format'          => $values['format'], 
    'coverage'        => $values['coverage'], 
    'right'           => $values['right']
  ))->execute();
  */

  $date = (object) $item->date;

  $item->created        = REQUEST_TIME;
  $item->changed        = REQUEST_TIME;
  $item->status_changed = REQUEST_TIME;
  $item->subject_en     = implode(',', array_filter($item->subject_en));
  $item->subject_sub    = implode(',', array_filter($item->subject_sub));

  // Dublin core
  $item->dc_description    = $item->description_sub.','.$item->description_en;
  $item->dc_subject        = $item->subject_sub.','.$item->subject_en;
  $item->dc_creator        = implode(',', array_filter($item->dc_creator));
  $item->dc_publisher      = implode(',', array_filter($item->dc_publisher));
  $item->dc_contributor    = implode(',', array_filter($item->dc_contributor));
  $item->dc_date           = mktime(0, 0, 0, $date->month, $date->day, $date->year);
  $item->dc_format         = $file->filemime;

  drupal_write_record('open_journal_journal', $item);

  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
}