<?php

function open_journal_people_form($form, &$form_state, $jid, $uid, $is_active) {
  
  $form['jid'] = array(
    '#type' => 'hidden',
    '#value' => $jid,
    //'#access' => FALSE,
  );
  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $uid,
    //'#access' => FALSE,
  );
  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => $is_active? 'deactive': 'active',
    //'#access' => FALSE,
  );
  
  $button_class = $is_active? 'active-people-btn': 'inactive-people-btn';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $is_active? 'active': 'deactive',
    '#attributes' => array('class' => array($button_class))
  );
  
  return $form;
}

function open_journal_people_form_submit($form, &$form_state) {
  
  $jid = $form_state['input']['jid'];
  $uid = $form_state['input']['uid'];
  $operation = $form_state['input']['operation'];
    
  if ($operation == 'deactive') {
    open_journal_journal_people_deactive($jid, $uid);
  }
  else {
    open_journal_journal_people_active($jid, $uid);
  }
  
  drupal_set_message(t('The people has been !s.', array('!s' => $operation)));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/people');
}