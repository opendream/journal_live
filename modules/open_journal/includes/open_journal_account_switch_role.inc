<?php

function open_journal_account_switch_role($role) {
  global $user;
  $account = user_load($user->uid);
  $user->roles = $account->roles;
  dpm($user->roles);

  if ($user->roles[$role->rid]) {
    $account = user_load($user->uid);
    user_save($account, array('data' => array('current_role' => $role->rid)));
  }

  drupal_goto();
}