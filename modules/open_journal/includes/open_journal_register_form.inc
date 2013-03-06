<?php
function open_journal_register_form($form, &$form_state) {
  global $user;

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

  $form['#attached']['library'][] = array('system', 'jquery.cookie');
  $form['#attributes']['class'][] = 'user-info-from-cookie';

  // Start with the default user account fields.
  open_journal_account_form($form, $form_state);

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
    '#value' => t('Register'),
  );
  $form['actions']['login'] = array(
    '#markup' => l(t('Login'), OPEN_JOURNAL_PREFIX_PATH.'/login', array('attributes' => array('class' => array('link-login')))),
  );
  $form['actions']['forget'] = array(
    '#markup' => l(t('Forget password'), 'user/password', array('attributes' => array('class' => array('link-forget')))),
  );

  $form['#validate'][] = 'user_register_validate';
  // Add the final user registration form submit handler.
  $form['#submit'][] = 'user_register_submit';

  return $form;

}






function open_journal_account_form(&$form, &$form_state) {
  global $user;

  $account = $form['#user'];
  $register = ($form['#user']->uid > 0 ? FALSE : TRUE);

  if ($register) {
    $form['title'] = array(
      '#markup' => '<h2>'.t('Register').'</h2>',
      '#weight' => -50
    );  
  }


  $admin = user_access('administer users');

  $form['#validate'][] = 'user_account_form_validate';

  // Account information.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#weight' => -10,
  );
  // Only show name field on registration form or user can change own username.
  $form['account']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#description' => t('Username should be a pretended. ex: mickymouse'),
    '#required' => TRUE,
    '#attributes' => array('class' => array('username')),
    '#default_value' => (!$register ? $account->name : ''),
    '#access' => ($register || ($user->uid == $account->uid && user_access('change own username')) || $admin),
    '#weight' => -10,
  );

  $form['account']['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#maxlength' => EMAIL_MAX_LENGTH,
    '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
    '#required' => TRUE,
    '#default_value' => (!$register ? $account->mail : ''),
  );

  // Display password field only for existing users or when user is allowed to
  // assign a password during registration.

  // TODO: Check when reuse to update profile form
  if (!$register) {
    $form['account']['pass'] = array(
      '#type' => 'password_confirm',
      '#size' => 25,
      '#description' => t('To change the current user password, enter the new password in both fields.'),
    );
    // To skip the current password field, the user must have logged in via a
    // one-time link and have the token in the URL.
    $pass_reset = isset($_SESSION['pass_reset_' . $account->uid]) && isset($_GET['pass-reset-token']) && ($_GET['pass-reset-token'] == $_SESSION['pass_reset_' . $account->uid]);
    $protected_values = array();
    $current_pass_description = '';
    // The user may only change their own password without their current
    // password if they logged in via a one-time login link.
    if (!$pass_reset) {
      $protected_values['mail'] = $form['account']['mail']['#title'];
      $protected_values['pass'] = t('Password');
      $request_new = l(t('Request new password'), 'user/password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
      $current_pass_description = t('Enter your current password to change the %mail or %pass. !request_new.', array('%mail' => $protected_values['mail'], '%pass' => $protected_values['pass'], '!request_new' => $request_new));
    }
    // The user must enter their current password to change to a new one.
    if ($user->uid == $account->uid) {
      $form['account']['current_pass_required_values'] = array(
        '#type' => 'value',
        '#value' => $protected_values,
      );
      $form['account']['current_pass'] = array(
        '#type' => 'password',
        '#title' => t('Current password'),
        '#size' => 25,
        '#access' => !empty($protected_values),
        '#description' => $current_pass_description,
        '#weight' => -5,
        '#attributes' => array('autocomplete' => 'off'),
      );
      $form['#validate'][] = 'user_validate_current_pass';
    }
  }


  if ($admin) {
    $status = isset($account->status) ? $account->status : 1;
  }
  else {
    $status = $register ? variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL) == USER_REGISTER_VISITORS : $account->status;
  }

  $form['account']['status'] = array(
    '#type' => 'hidden',
    '#value' => $status,
    '#access' => FALSE,
  );

  $roles = array_map('check_plain', user_roles(TRUE));
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in Form API not supporting a single disabled
  // checkbox within a set of checkboxes.
  // @todo This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['account']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => (!$register && isset($account->roles) ? array_keys($account->roles) : array()),
    '#options' => $roles,
    '#access' => FALSE,
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );


  $form['account']['notify'] = array(
    '#type' => 'hidden',
    '#value' => FALSE,
    '#access' => FALSE,
  );

  // Signature.
  $form['signature_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Signature settings'),
    '#weight' => 1,
    '#access' => FALSE,
  );

  $form['signature_settings']['signature'] = array(
    '#type' => 'text_format',
    '#title' => t('Signature'),
    '#default_value' => isset($account->signature) ? $account->signature : '',
    '#description' => t('Your signature will be publicly displayed at the end of your comments.'),
    '#format' => isset($account->signature_format) ? $account->signature_format : NULL,
    '#access' => FALSE,
  );
  
  $form['data'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Information'),
    '#description' => t('This infomation display for you only.'),
  );
  $form['data']['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => isset($account->data) && isset($account->data['firstname'])? $account->data['firstname']: '',
    '#description' => t('Ex : Sommay'),
  );
  $form['data']['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => isset($account->data) && isset($account->data['lastname'])? $account->data['lastname']: '',
    '#description' => t('Ex : Dejai'),
  );
  

  // Picture/avatar.
  $form['picture'] = array(
    '#type' => 'fieldset',
    '#title' => t('Picture'),
    '#weight' => 1,
    '#access' => (!$register && variable_get('user_pictures', 0)),
  );
  $form['picture']['picture'] = array(
    '#type' => 'value',
    '#value' => isset($account->picture) ? $account->picture : NULL,
  );
  $form['picture']['picture_current'] = array(
    '#markup' => theme('user_picture', array('account' => $account)),
  );
  $form['picture']['picture_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete picture'),
    '#access' => !empty($account->picture->fid),
    '#description' => t('Check this box to delete your current picture.'),
  );
  $form['picture']['picture_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload picture'),
    '#size' => 48,
    '#description' => t('Your virtual face or picture. Pictures larger than @dimensions pixels will be scaled down. <br /> <strong>! This picture display for you only.</strong>', array('@dimensions' => variable_get('user_picture_dimensions', '85x85'))) . ' ' . filter_xss_admin(variable_get('user_picture_guidelines', '')),
  );
  $form['#validate'][] = 'user_validate_picture';
}
