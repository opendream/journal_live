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
    $form['main']['abstract_sub'] = array(
      '#title' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_LABEL,
      '#type' => 'textarea',
      '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
      '#required' => OPEN_JOURNAL_SUB_LANG_ABSTRACT_REQUIRED,
    );
    $form['main']['keywords_sub'] = array(
      '#title' => OPEN_JOURNAL_SUB_LANG_KEYWORDS_LABEL,
      '#type' => 'textfield',
      '#required' => OPEN_JOURNAL_SUB_LANG_KEYWORDS_REQUIRED,
    );
  }

  $form['main']['abstract_en'] = array(
    '#title' => t('Abstract'),
    '#type' => 'textarea',
    '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH)),
    '#required' => OPEN_JOURNAL_ABSTRACT_REQUIRED,
  );
  $form['main']['keywords_en'] = array(
    '#title' => t('Keywords'),
    '#type' => 'textfield',
    '#required' => OPEN_JOURNAL_KEYWORDS_REQUIRED,
  );

  $form['main']['file'] = array(
    '#type' => 'file',
    '#title' => t('File'),
    '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
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

  $form['#submit'][] = 'open_journal_add_submit';


  // Sidebar form

  $form['data'] = array('#type' => 'container');
  $form['data']['data_title'] = array('#markup' => '<h3>'.t('Data').'</h3>');



  $multi_textfield = array();
  for ($i = 0; $i < OPEN_JOURNAL_MAX_MULTI_TEXTFIELD; $i++) {
    $multi_textfield[] = array(
      '#type' => 'textfield',
      '#attributes' => array('class' => array('multi-textfield', 'multi-textfield-'.$i))
    );
  }
  $multi_textfield[] = array('#markup' => '<button class="add-multi-textfield">'.t('Add').'</button>');

  $form_group = array(
    '#type' => 'fieldset',
    'contributor' => array(
      '#type' => 'fieldset',
      '#title' => t('Contributors'),
      '#tree' => TRUE,
      $multi_textfield,
    ),
    'creator' => array(
      '#type' => 'fieldset',
      '#title' => t('Creators'),
      '#tree' => TRUE,
      $multi_textfield
    ),

  );

  $form['data']['group'] = $form_group;


  return $form;
}