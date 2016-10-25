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