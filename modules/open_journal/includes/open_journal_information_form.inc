<?php

function open_journal_information_form($form, &$form_state) {

  // Main form

  $form['main'] = array('#type' => 'container');

  $form['main']['main_title'] = array('#markup' => '<h2>'.t('Information').'</h2>');

  $form['main']['abstract_sub'] = array(
    '#title' => t('!s Abstract', array('!s' => OPEN_JOURNAL_SUB_LANG)),
    '#type' => 'textarea',
    '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH))
  );

  $form['main']['abstract_en'] = array(
    '#title' => t('English Abstract'),
    '#type' => 'textarea',
    '#description' => t('No more than !d words please!', array('!d' => OPEN_JOURNAL_ABSTRACT_MAX_LENGTH))
  );

  $form['main']['file'] = array(
    '#type' => 'file',
    '#title' => t('File'),
    '#description' => t('Allowed file types: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FILE_TYPE)),
  );

  $form['main']['actions'] = array('#type' => 'actions');
  $form['main']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER']),
  );

  $form['#submit'][] = 'open_journal_add_submit';


  // Sidebar form

  $form['data'] = array('#type' => 'container');
  $form['data']['data_title'] = array('#markup' => '<h3>'.t('Data').'</h3>');

  $form_group = array(
    '#type' => 'fieldset',
    'status' => array(
      '#type' => 'textfield'
    ),
    'title' => array(),

  );

  $form['data']['group'] = $form_group;

  return $form;
}