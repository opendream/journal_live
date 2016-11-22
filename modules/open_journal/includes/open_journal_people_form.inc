<?php

function open_journal_people_form($form, &$form_state, $jid, $uid, $is_active) {
  
  $form['#attached']['js'][] = drupal_get_path('module', 'open_journal').'/js/open_journal_toggle.js';
  
  $form['jid'] = array(
    '#type' => 'hidden',
    '#value' => $jid,
  );
  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $uid,
  );
  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => $is_active? 'deactivate': 'activate',
  );
  
  $button_class = array('toggle-people-btn');
  $button_class[] = $is_active? 'active-people-btn': 'inactive-people-btn';
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $is_active? 'activate': 'deactivate',
    '#attributes' => array('class' => $button_class)
  );
  
  return $form;
}

function open_journal_people_form_submit($form, &$form_state) {
  
  $jid = $form_state['input']['jid'];
  $uid = $form_state['input']['uid'];
  $operation = $form_state['input']['operation'];
    
  if ($operation == 'deactivate') {
    //open_journal_journal_people_deactive($jid, $uid);
    open_journal_journal_people_active($jid, $uid, 0);
  }
  else {
    open_journal_journal_people_active($jid, $uid);
  }
  
  drupal_set_message(t('The people has been !s.', array('!s' => $operation)));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/people');
}


function open_journal_people_create_form($form, &$form_state, $journal) {

  module_load_include('inc', 'user', 'user.pages');
  module_load_include('inc', 'open_journal', 'includes/open_journal_register_form');
  
  global $user;
  
  //$form['#attached']['js']['open_journal_compact'] = drupal_add_js(drupal_get_path('module', 'open_journal').'/js/open_journal_compact.js');
  
  $admin = user_access('administer users');

  // Pass access information to the submit handler. Running an access check
  // inside the submit function interferes with form processing and breaks
  // hook_form_alter().
  $form['administer_users'] = array(
    '#type' => 'value',
    '#value' => $admin,
  );


  $form['#user'] = drupal_anonymous_user();
  $form['#user_category'] = 'register';

  //$form['#attached']['library'][] = array('system', 'jquery.cookie');
  //$form['#attributes']['class'][] = 'user-info-from-cookie';

  // Start with the default user account fields.
  open_journal_account_form($form, $form_state);

  if ($_GET['role']) {
    $role = user_role_load($_GET['role']);
  }

  // Alter 
  $form['title']['#markup'] = '<h2 class="sub-page-title">'.t('Add !role to journal', array('!role' => $role->name)).'</h2>';
  
  if ($_GET['role']) {
    $form['account']['roles']['#default_value'] = array($_GET['role']);
  }
  else {
    $form['account']['roles']['#access'] = TRUE;
  }

  $form['jid'] = array(
    '#type' => 'hidden',
    '#value' => $journal->jid,
    '#access' => FALSE
  );

  $form['account']['name']['#type'] = 'hidden';
  $form['account']['name']['#value'] = 'user.'.REQUEST_TIME;

  $form['data']['firstname']['#required'] = TRUE;
  $form['data']['lastname']['#required'] = TRUE;

  // Attach field widgets, and hide the ones where the 'user_register_form'
  // setting is not on.

  foreach (field_info_instances('user', 'user') as $field_name => $instance) {
    if (empty($instance['settings']['user_register_form'])) {
      $form[$field_name]['#access'] = FALSE;
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add and activate'),
  );
  $form['actions']['forget'] = array(
    '#markup' => l(t('Cancel'), OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/people', array('attributes' => array('class' => array('link-forget')))),
  );

  $form['#validate'][] = 'open_journal_people_create_form_validate';

  $form['#validate'][] = 'user_register_validate';
  // Add the final user registration form submit handler.
  $form['#submit'][] = 'user_register_submit';
  $form['#submit'][] = 'open_journal_people_create_form_submit';

  return $form;

}

function open_journal_people_create_form_validate($form, &$form_state) {
  $form_state['values']['name'] = $form_state['values']['mail'];
}

function open_journal_people_create_form_submit($form, &$form_state) {
  $jid = $form_state['values']['jid'];
  open_journal_journal_people_active($jid, $form_state['values']['uid']);
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/people');
}