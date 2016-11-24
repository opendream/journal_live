<?php

function open_journal_information_form($form, &$form_state, $data=NULL) {

  global $user;
    
  $mpath = drupal_get_path('module', 'open_journal');
  
  $form['#attached']['js'][] = $mpath.'/js/open_journal_multi_textfield.js';
  $form['#attached']['js'][] = $mpath.'/js/open_journal_author.js';
  $form['#attached']['js'][] = $mpath.'/js/jquery.qtip.min.js';
  $form['#attached']['js'][] = $mpath.'/js/open_journal_help.js';
  
  $fs = open_journal_get_field_settings();

  // Main form
  $form['main'] = array('#type' => 'container');

  $form['main']['main_title'] = array('#markup' => '<h2>'.t('Information').'</h2>');
  
  
  // Add sub title 
  $title_label = 'Title';
  if(OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['main']['title_sub'] = array(
      '#title' => open_journal_label_help(OPEN_JOURNAL_SUB_LANG_TITLE_LABEL, 'Name of the article in '.OPEN_JOURNAL_SUB_LANG_FULL_TEXT.' language', TRUE),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#maxlength' => 1028,
      '#default_value' => open_journal_get_field($data, 'title_sub'),
      '#access' => $fs['title_sub'],
    );
    $title_label .= ' ('.OPEN_JOURNAL_LANG_FULL_TEXT.')';
  }
  
  $form['main']['title_en'] = array(
    '#title' => open_journal_label_help($title_label, 'Name of the article in '.OPEN_JOURNAL_LANG_FULL_TEXT.' language', TRUE),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 1028,
    '#default_value' => open_journal_get_field($data, 'title_en'),
    '#access' => $fs['title_en'],
  );

  
  $abstract_label = 'Abstract';
  
  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['main']['description_sub'] = array(
      '#title' => open_journal_label_help(OPEN_JOURNAL_SUB_LANG_ABSTRACT_LABEL, OPEN_JOUNAL_HELP_DESCRIPTION_SUB, TRUE),
      '#type' => 'textarea',
      '#description' => t('No more than !d words.', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
      '#required' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_REQUIRED,
      '#default_value' => open_journal_get_field($data, 'description_sub'),
      '#rows' => 10,
      '#access' => $fs['dc_description'],
    );
    
    $abstract_label .= ' ('.OPEN_JOURNAL_LANG_FULL_TEXT.')';
  }
  
  
  $form['main']['description_en'] = array(
    '#title' => open_journal_label_help($abstract_label, 'Summary of the article', TRUE),
    '#type' => 'textarea',
    '#description' => t('No more than !d words.', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
    '#required' => OPEN_JOURNAL_ABSTRACT_REQUIRED,
    '#default_value' => open_journal_get_field($data, 'description_en'),
    '#rows' => 10,
    '#access' => $fs['dc_description'],
  );

  if(isset($data->jid)) {

    $latest_file = open_journal_get_file_list($data->jid, 1);
    $latest_file = $latest_file[0];

    $form['main']['file_link'] = array(
      '#markup' => '<div class="form-type-managed-file">Latest file : '.l($latest_file->filename, file_create_url($latest_file->uri)).'<div class="description">'.t('When you upload a file, the latest file will appear at the top.').'</div></div>'
    );
    
    $submit_text = t('Save changes');
  }
  else {
    $form['main']['file'] = array(
      '#type' => 'managed_file',
      '#title' => open_journal_label_help('File', 'Upload a full-text file here', TRUE),
      '#description' => t('File types allowed: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
      '#upload_validators' => array(
        'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FILE_TYPE),
        //'file_validate_size' => array(2 * 1048576),\
      ),
      '#upload_location' => "public://journal/",
      '#required' => TRUE,
    );

    $form['main']['guideline'] = array(
      '#title' => t('I have read the submission guidelines.').' '.l(t('Download guideline'), ''),
      '#type' => 'checkbox',
      '#required' => TRUE,
    );
    
    $submit_text = t('Submit');
  }

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  $form['main']['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );


  // Sidebar form

  $form['data'] = array('#type' => 'container');
  $form['data']['data_title'] = array('#markup' => '<h3>'.t('Data').'</h3>');

  $form['data']['group'] = array('#type' => 'fieldset');
  $form['data']['group']['author'] = array('#type' => 'fieldset');

//open_journal_multi_textfield($title='' , $values = FALSE, $name, $add_more_text='Add another', $add_more_text_show=TRUE ,$initail_show_field=2)
//($title='' , $values = FALSE, $name, $add_more_text='Add another', $add_more_text_show=TRUE ,$initail_show_field=2, $data=NULL, $required=FALSE)

  
  $author_label = 'Author';
  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['author']['creator_sub'] = open_journal_multi_textfield(OPEN_JOURNAL_SUB_LANG_AUTHOR_LABEL ,open_journal_get_field($data, 'creator_sub'), 'dc_creator', 'Add co-author', TRUE, 1, NULL, TRUE);
    $author_label .= ' ('.OPEN_JOURNAL_LANG_FULL_TEXT.')';
  }
    
  $form['data']['group']['author']['creator_en'] = open_journal_multi_textfield($author_label ,open_journal_get_field($data, 'creator_en'), 'dc_creator', 'Add co-author', TRUE, 1, NULL, TRUE);

  $form['data']['group']['author']['creator_email'] = open_journal_multi_textfield('Author (email)' ,open_journal_get_field($data, 'creator_email'), 'dc_creator.email', 'Add co-author', TRUE, 1, NULL, TRUE);
  
  
  $keywords_label = 'Keywords';
  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['subject_sub'] = open_journal_multi_textfield(OPEN_JOURNAL_SUB_LANG_KEYWORDS_LABEL.'',open_journal_get_field($data, 'subject_sub'), 'dc_subject', 'Add another', TRUE, 2, NULL, TRUE);
    $keywords_label .= ' ('.OPEN_JOURNAL_LANG_FULL_TEXT.')';
  }
  $form['data']['group']['subject_en'] = open_journal_multi_textfield($keywords_label,open_journal_get_field($data, 'subject_en'), 'dc_subject', 'Add another', TRUE, 2, NULL, TRUE);
  
 

  

  $form['data']['group']['address'] = array(
    '#title' => open_journal_label_help('Offices', 'Work address for principal researcher', FALSE),
    '#type' => 'textarea',
    '#default_value' => open_journal_get_field($data, 'address'),
    '#rows' => 3,
    '#access' => $fs['address'],
  );

  $form['data']['group']['dc_publisher'] = open_journal_multi_textfield('Publisher' ,open_journal_get_field($data, 'dc_publisher'), 'dc_publisher', '', FALSE, 1);
  $form['data']['group']['dc_contributor'] = open_journal_multi_textfield('Contributors', open_journal_get_field($data, 'dc_contributor'), 'dc_contributor', '', FALSE, 1);

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
    '#title' => open_journal_label_help('Date', 'Date of submission'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#access' => $fs['dc_date'],
  );
  $form['data']['group']['date']['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(date('Y')-10,date('Y')),
    '#default_value' => $year,
    '#suffix' => '<span>/</span>',
  );
  $form['data']['group']['date']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,12),
    '#default_value' => $month,
    '#suffix' => '<span>/</span>',

  );
  $form['data']['group']['date']['day'] = array(
    '#title' => t('Day'),
    '#type' => 'select',
    '#options' => open_journal_get_date_option(1,30),
    '#default_value' => $day,
  );

  if (OPEN_JOURNAL_SUB_LANG_ENABLE) {
    $form['data']['group']['dc_language'] = array(
      '#title' => open_journal_label_help('Language', 'Language used in the article'),
      '#type' => 'select',
      '#options' => array(
        OPEN_JOURNAL_SUB_LANG_CODE => t(OPEN_JOURNAL_SUB_LANG_TEXT),
        'en' => 'en',
      ),
      '#default_value' => open_journal_get_field($data, 'dc_language'),
      '#access' => $fs['dc_language'],
    );
  }
  else {
    $form['data']['group']['dc_language'] = array(
      '#type' => 'hidden',
      '#value' => OPEN_JOURNAL_SUB_LANG_ENABLE,
      '#access' => FALSE,
    );
  }
  
  $form['data']['group']['dc_coverage'] = array(
    '#title' => open_journal_label_help('Coverage', 'e.g. geographical area, time period, sample size, characteristics of research sample'),
    '#type' => 'textfield',
    '#default_value' => open_journal_get_field($data, 'dc_coverage'),
    '#access' => $fs['dc_coverage'],
  );
  
  $form['data']['group']['dc_type'] = array(
    '#title' => open_journal_label_help('Type', 'Type of article'),
    '#type' => 'textfield',
    '#default_value' => open_journal_get_field($data, 'dc_type', 'Article'),
    '#access' => user_access('access contributor process') && $fs['dc_identifier'],
  );

  $form['data']['group']['dc_format'] = array(
    '#title' => open_journal_label_help('Format', 'Format of article'),
    '#type' => 'textfield',
    '#default_value' => open_journal_get_field($data, 'dc_format', 'TEXT/HTML'),
    '#access' => user_access('access contributor process') && $fs['dc_identifier'],
  );

  $form['data']['group']['dc_right'] = array(
    '#title' => open_journal_label_help('Right', 'Information about rights held in and over the resource'),
    '#type' => 'textfield',
    '#default_value' => open_journal_get_field($data, 'dc_right'),
    '#access' => user_access('access contributor process') && $fs['dc_identifier'],
  );
  
  $form['data']['group']['dc_identifier'] = array(
    '#title' => open_journal_label_help('Final URL', 'For librarian only'),
    '#type' => 'textfield',
    '#access' => user_access('access contributor process') && $fs['dc_identifier'],
    '#default_value' => open_journal_get_field($data, 'dc_identifier'),
  );

  // Hidden field

 

  

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


function open_journal_multi_textfield($title='' , $values = FALSE, $name, $add_more_text='Add another', $add_more_text_show=TRUE ,$initail_show_field=2, $data=NULL, $required=FALSE) {
  
  $fs = open_journal_get_field_settings();
  $help_map = array(
    'dc_subject' => 'Words or phrases relevant to the article',
    'dc_publisher' => 'Health Systems Research Institute',
    'dc_contributor' => 'Funding organizations',
    'dc_creator' => 'Name of principal researcher',
    'dc_creator.email' => 'Email of principal researcher',
  );

  $help_text = isset($help_map[$name])? $help_map[$name]: 'Help me please.';

  $multi_textfield = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#access' => $fs[$name],
  );

  for ($i = 0; $i < OPEN_JOURNAL_MAX_MULTI_TEXTFIELD; $i++) {
    if($i < $initail_show_field) {
      $alt = 'show';
    }
    else {
      $alt = 'hide';
      
    }
    if(!isset($values[$i]) && $name == 'dc_publisher' && $i == 0) {
      $values[$i] = OPEN_JOURNAL_DEFAULT_PUBLISHER_NAME;

    } 
    elseif(!isset($values[$i])) {
      $values[$i] = '';
    }

    // hardcode
    
    // Check required on first input field
    if($i == 0) {
      $multi_textfield[] = array(
        '#type' => 'textfield',
        '#title' => open_journal_label_help($title, $help_text),
        '#attributes' => array('class' => array('multi-textfield', 'multi-textfield-'.$i) ,'alt' => $alt),
        '#default_value' => $values[$i],
        '#required' => $required,
      );
    }
    else {
      $multi_textfield[] = array(
        '#type' => 'textfield',
        '#attributes' => array('class' => array('multi-textfield', 'multi-textfield-'.$i) ,'alt' => $alt),
        '#default_value' => $values[$i],
      );
    }
    
    
  }
  if($add_more_text_show) {
    $multi_textfield[] = array('#markup' => '<button class="add-multi-textfield"> '.t($add_more_text).'</button>');
  }

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

  $is_new = (isset($data->jid) && $data->jid)? FALSE: TRUE;
  
  if ($is_new) {
    open_journal_journal_save($data, TRUE);
    drupal_set_message(t('Your submission is completed. It is now waiting for information review.'));
    drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$data->jid.'/overview');
  }
  else {
    open_journal_journal_save($data);
    drupal_set_message(t('This article information has been updated.'));
    drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$data->jid.'/overview');
  }

}

function open_journal_label_help($label='', $help_text='', $leftside=FALSE) {
  if ($leftside) {
    return '<span class="label-text">'.t($label).'</span><span class="label-help tip-right">'.$help_text.'</span>';
  }
  else {
    return '<span class="label-text">'.t($label).'</span><span class="label-help">'.$help_text.'</span>';
  }
}
