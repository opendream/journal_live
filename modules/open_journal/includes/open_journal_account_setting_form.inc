<?php

function open_journal_account_setting_form($form, &$form_state) {

  module_load_include('inc', 'user', 'user.pages');
  module_load_include('inc', 'open_journal', 'includes/open_journal_register_form');

  global $user;

  $category = 'account';
  $account = user_load($user->uid);
  


  // During initial form build, add the entity to the form state for use during
  // form building and processing. During a rebuild, use what is in the form
  // state.
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  

  // @todo Legacy support. Modules are encouraged to access the entity using
  //   $form_state. Remove in Drupal 8.
  $form['#user'] = $account;
  $form['#user_category'] = $category;

  if ($category == 'account') {
    open_journal_account_form($form, $form_state);
    // Attach field widgets.
    $langcode = entity_language('user', $account);
    field_attach_form('user', $account, $form, $form_state, $langcode);
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($category == 'account') {
    $form['actions']['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel account'),
      '#submit' => array('user_edit_cancel_submit'),
      '#access' => $account->uid > 1 && (($account->uid == $user->uid && user_access('cancel account')) || user_access('administer users')),
    );
  }

  $form['#validate'][] = 'user_profile_form_validate';
  // Add the final user profile form submit handler.
  $form['#submit'][] = 'user_profile_form_submit';


  return $form;
}