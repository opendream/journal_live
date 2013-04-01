<?php
function open_journal_login_form($form_state) {

  global $user;

  // Display login form:
    
  $form['#attached']['js']['open_journal_compact'] = drupal_add_js(drupal_get_path('module', 'open_journal').'/js/open_journal_compact.js');

  $form['title'] = array(
    '#markup' => '<h2>'.t('Login').'</h2>',
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Login with Username or E-mail'),
    '#size' => 60,
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#required' => TRUE,
  );

  $form['pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Login'),
  );
  $form['actions']['register'] = array(
    '#markup' => l(t('Register'), OPEN_JOURNAL_PREFIX_PATH.'/register', array('attributes' => array('class' => array('link-register')))),
  );
  $form['actions']['forget'] = array(
    '#markup' => l(t('Forget password'), 'journal/password', array('attributes' => array('class' => array('link-forget')))),
  );

  $form['#theme'] = 'open_journal_login_form';
  $form['#validate'] = array('open_journal_login_validate');

  return $form;
}

function open_journal_login_form_submit($form, &$form_state) {
  global $user;
  $user = user_load($form_state['uid']);
  
  
  $form_state['redirect'] = 'journal';

  user_login_finalize($form_state);
  
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH);

}

function open_journal_login_validate($form, &$form_state) {

  $account = db_query("SELECT * FROM {users} WHERE (name = :name OR mail = :name) AND status = 1", array(':name' => $form_state['values']['name']))->fetchObject();

  // copy from user_login_name_validate
  if (isset($form_state['values']['name']) && $account && user_is_blocked($account->name)) {
    // Blocked in user administration.
    form_set_error('name', t('The username or email %name has not been activated or is blocked.', array('%name' => $form_state['values']['name'])));
  }



  // copy from user_login_authenticate_validate
  $password = trim($form_state['values']['pass']);
  if (!empty($form_state['values']['name']) && !empty($password)) {
    // Do not allow any login from the current user's IP if the limit has been
    // reached. Default is 50 failed attempts allowed in one hour. This is
    // independent of the per-user limit to catch attempts from one IP to log
    // in to many different user accounts.  We have a reasonably high limit
    // since there may be only one apparent IP for all users at an institution.
    if (!flood_is_allowed('failed_login_attempt_ip', variable_get('user_failed_login_ip_limit', 50), variable_get('user_failed_login_ip_window', 3600))) {
      $form_state['flood_control_triggered'] = 'ip';
      return;
    }

    if ($account) {
      if (variable_get('user_failed_login_identifier_uid_only', FALSE)) {
        // Register flood events based on the uid only, so they apply for any
        // IP address. This is the most secure option.
        $identifier = $account->uid;
      }
      else {
        // The default identifier is a combination of uid and IP address. This
        // is less secure but more resistant to denial-of-service attacks that
        // could lock out all users with public user names.
        $identifier = $account->uid . '-' . ip_address();
      }
      $form_state['flood_control_user_identifier'] = $identifier;

      // Don't allow login if the limit for this user has been reached.
      // Default is to allow 5 failed attempts every 6 hours.
      if (!flood_is_allowed('failed_login_attempt_user', variable_get('user_failed_login_user_limit', 5), variable_get('user_failed_login_user_window', 21600), $identifier)) {
        $form_state['flood_control_triggered'] = 'user';
        return;
      }
      
      // We are not limited by flood control, so try to authenticate.
      // Set $form_state['uid'] as a flag for user_login_final_validate().
      $form_state['uid'] = user_authenticate($account->name, $password);
      
    }

  }





  // copy from user_login_final_validate 
  if (empty($form_state['uid'])) {
    // Always register an IP-based failed login event.
    flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
    // Register a per-user failed login event.
    if (isset($form_state['flood_control_user_identifier'])) {
      flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
    }

    if (isset($form_state['flood_control_triggered'])) {
      if ($form_state['flood_control_triggered'] == 'user') {
        form_set_error('name', format_plural(variable_get('user_failed_login_user_limit', 5), 'Sorry, there has been more than one failed login attempt for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', 'Sorry, there have been more than @count failed login attempts for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('journal/password'))));
      }
      else {
        // We did not find a uid, so the limit is IP-based.
        form_set_error('name', t('Sorry, too many failed login attempts from your IP address. This IP address is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('journal/password'))));
      }
    }
    else {
      form_set_error('name', t('Sorry, unrecognized username or password. <a href="@password">Have you forgotten your password?</a>', array('@password' => url('journal/password'))));
      if ($account) {
        watchdog('user', 'Login attempt failed for %user.', array('%user' => $account->name));
      }
      else {
        watchdog('user', 'Login attempt failed for user.');

      }
    }
  }
  elseif (isset($form_state['flood_control_user_identifier'])) {
    // Clear past failures for this user so as not to block a user who might
    // log in and out more than once in an hour.
    flood_clear_event('failed_login_attempt_user', $form_state['flood_control_user_identifier']);
  }







}
