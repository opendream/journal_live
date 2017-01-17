<?php

// Reject

function open_journal_reject_sidebar_form($form, &$form_state, $journal, $template_list=FALSE) {
  $default_value = '';
  if ($template_list) {
    $default_value = $template_list[0]->body;
    $roles = array();
    foreach ($template_list[0]->role as $key => $value) {
      $roles[$value->rid] = $value->rid;
    }
    $form['tpid'] = array(
      '#type' => 'hidden',
      '#value' => $template_list[0]->tpid,
      '#access' => FALSE,
    );
    $form['template_title'] = array(
      '#type' => 'hidden',
      '#value' => $template_list[0]->template_title,
      '#access' => FALSE,
    );

    $form['roles'] = array(
      '#type' => 'hidden',
      '#value' => $roles,
      '#access' => FALSE,
    );

  }
	$form['reject_reason'] = array(
		'#type' => 'textarea',
    '#default_value' => $default_value,
	);

	$form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
    	'#type' => 'submit',
    	'#value' => t('Confirm'),
 	);
    $form['actions']['cancel'] = array(
    	'#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER'], array(
    		'attributes' => array('id' => 'reject-cancel-link')
    	)),
    );

	
	return $form;
}

function open_journal_reject_submit($jid, $reject_reason='', $roles=FALSE, $template_title=FALSE, $redirect=TRUE) {

  preg_match_all("^\[\[(.*?)\]\]^", $reject_reason, $fill_reject_reason, PREG_PATTERN_ORDER);

  if (count($fill_reject_reason) == 2) {
    $fill_reject_reason = implode("\n ", $fill_reject_reason[1]);
  }
  else {
    $fill_reject_reason = $reject_reason;
  }

  $sql = "UPDATE open_journal_journal SET reject = 1, status_changed = :status_changed, reject_reason = :reject_reason WHERE jid = ".$jid;
  db_query($sql, array(':status_changed' => REQUEST_TIME, ':reject_reason' => $fill_reject_reason));

  // Create new discussion
  $reject_reason = open_journal_trim($reject_reason)? $reject_reason:'We are sorry. If you want to more information to rejected, you can talk on this discussion.' ;
  $discussion = array(
    'jid' => $jid,
    'title' => t('This journal has been rejected.'),
    'description' => $reject_reason,
    
  );

  if ($template_title) {
    $discussion['title'] = $template_title;
  }
  else {
    $discussion['title'] = t('This journal has been rejected.');

  }

  if ($roles) {
    $discussion['roles'] = $roles;
  }

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  
  if ($redirect) {
    drupal_set_message('The article is rejected.');
    drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/discussion/'.$discussion->did);
  }
  
}

function open_journal_reject_sidebar_form_submit($form, &$form_state) {
	$reject_reason = $form['reject_reason']['#value'];
  $roles = $form['roles']['#value'];
  $template_title = $form['template_title']['#value'];
	$jid = $form_state['build_info']['args'][0]->jid;

  open_journal_reject_submit($jid, $reject_reason, $roles, $template_title);
	
}

function open_journal_not_reject_form($form, &$form_state) {
	global $user;

	if(!isset($user->data['not_show_reject'])) {
		$value = 0;
	}
	else {
		$value = $user->data['not_show_reject'];
	}

	$form['not_show_reject'] = array(
		'#type' => 'checkbox',
		'#title' => 'Include rejected articles',
		'#attributes' => array('class' => array('not_reject_checkbox')),
		'#default_value' => !$value,

	);

	$form['submit'] = array(
    	'#type' => 'submit',
    	'#value' => t('submit'),
    	'#attributes' => array('id' => array('not_reject_btu')),
 	);
	return $form;
}
function open_journal_not_reject_form_submit($form, &$form_state) {
	global $user;

	$account = user_load($user->uid);
	user_save($account, array('data' => array('not_show_reject' => !$form_state['values']['not_show_reject'])));

	drupal_goto($_SERVER['HTTP_REFERER']);
}

// Withdraw
function open_journal_withdraw_sidebar_form($form, &$form_state, $journal, $template_list=FALSE) {
  $default_value = '';
  if ($template_list) {
    $default_value = $template_list[0]->body;
    $roles = array();
    foreach ($template_list[0]->role as $key => $value) {
      $roles[$value->rid] = $value->rid;
    }
    $form['tpid'] = array(
      '#type' => 'hidden',
      '#value' => $template_list[0]->tpid,
      '#access' => FALSE,
    );
    $form['template_title'] = array(
      '#type' => 'hidden',
      '#value' => $template_list[0]->template_title,
      '#access' => FALSE,
    );

    $form['roles'] = array(
      '#type' => 'hidden',
      '#value' => $roles,
      '#access' => FALSE,
    );

  }
  $form['withdraw_reason'] = array(
    '#type' => 'textarea',
    '#default_value' => $default_value,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Confirm'),
  );
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER'], array(
        'attributes' => array('id' => 'withdraw-cancel-link')
      )),
    );

  
  return $form;
}

function open_journal_withdraw_submit($jid, $withdraw_reason='', $roles=FALSE, $template_title=FALSE, $redirect=TRUE) {
  $sql = "UPDATE open_journal_journal SET withdraw = 1, status_changed = :status_changed, withdraw_reason = :withdraw_reason WHERE jid = ".$jid;
  db_query($sql, array(':status_changed' => REQUEST_TIME, ':withdraw_reason' => $withdraw_reason));

  // Create new discussion
  $withdraw_reason = open_journal_trim($withdraw_reason)? $withdraw_reason:'We are sorry. If you want to more information to withdrawn, you can talk on this discussion.' ;
  $discussion = array(
    'jid' => $jid,
    'title' => t('This journal has been withdrawn.'),
    'description' => $withdraw_reason,
    
  );

  if ($template_title) {
    $discussion['title'] = $template_title;
  }
  else {
    $discussion['title'] = t('This journal has been withdrawn.');

  }

  if ($roles) {
    $discussion['roles'] = $roles;
  }

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  
  if ($redirect) {
    drupal_set_message('The article is withdrawn.');
    drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/discussion/'.$discussion->did);
  }
  
}

function open_journal_withdraw_sidebar_form_submit($form, &$form_state) {
  $withdraw_reason = $form['withdraw_reason']['#value'];
  $roles = $form['roles']['#value'];
  $template_title = $form['template_title']['#value'];
  $jid = $form_state['build_info']['args'][0]->jid;

  open_journal_withdraw_submit($jid, $withdraw_reason, $roles, $template_title);
  
}

function open_journal_not_withdraw_form($form, &$form_state) {
  global $user;

  if(!isset($user->data['not_show_withdraw'])) {
    $value = 0;
  }
  else {
    $value = $user->data['not_show_withdraw'];
  }

  $form['not_show_withdraw'] = array(
    '#type' => 'checkbox',
    '#title' => 'Include withdrawn articles',
    '#attributes' => array('class' => array('not_withdraw_checkbox')),
    '#default_value' => !$value,

  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('submit'),
      '#attributes' => array('id' => array('not_withdraw_btu')),
  );
  return $form;
}
function open_journal_not_withdraw_form_submit($form, &$form_state) {
  global $user;

  $account = user_load($user->uid);
  user_save($account, array('data' => array('not_show_withdraw' => !$form_state['values']['not_show_withdraw'])));

  drupal_goto($_SERVER['HTTP_REFERER']);
}

// Approve
function open_journal_approve_sidebar_form($form, &$form_state, $journal , $next_status, $template_list=FALSE) {

  global $user;
  drupal_add_js(drupal_get_path('module', 'open_journal') . '/js/open_journal_status_sidebar.js');
  drupal_add_js(drupal_get_path('module', 'open_journal') . '/js/jquery.qtip.min.js');
  
  if($template_list) {
    $default_value = $template_list[0]->body;
    $roles = array();
    foreach ($template_list[0]->role as $key => $value) {
      $roles[$value->rid] = $value->rid;
    }
    // Include creator user roles to discussion
    $access_roles = user_roles(FALSE, 'send email template');
    foreach ($user->roles as $key => $value) {
      if (isset($access_roles[$key])) {
        $roles[$key] = $key;
      }
    }

    $form['approve_reason'] = array(
      '#type' => 'textarea',
      '#default_value' => $default_value,
    );

    $allow_file_type = OPEN_JOURNAL_ALLOWED_FILE_TYPE;
    if (open_journal_user_access('access contributor process', $user, TRUE) || in_array($journal->sid, array(11, 12, 13, 2, 3))) {
      $allow_file_type = OPEN_JOURNAL_ALLOWED_FINAL_STATUS_FILE_TYPE;
    }

    $form['fid'] = array(
      '#type' => 'managed_file',
      '#title' => t('File'),
      '#description' => t('File type allowed: %s',array('%s' => $allow_file_type)),
      '#upload_validators' => array(
        'file_validate_extensions' => array($allow_file_type),
      ),
      '#upload_location' => "public://journal/",
    );

    // Multiple upload file
    // Todo rit1 remove hardcode
    if (open_journal_user_access('access contributor process', $user, TRUE)) {
      if (!$form_disabled) {
        $form['files_upload'] = array( 
          '#type' => 'plupload', 
          '#title' => t('Upload other files.'), 
          '#description' => t('File type allowed: %s',array('%s' => OPEN_JOURNAL_ALLOWED_FINAL_STATUS_FILE_TYPE)),
          '#upload_validators' => array(
            'file_validate_extensions' => array(OPEN_JOURNAL_ALLOWED_FINAL_STATUS_FILE_TYPE),
            //'file_validate_size' => array(2 * 1048576),
          ),
          '#size' => 50, 
          '#upload_location' => "public://journal/",
          '#autoupload' => TRUE,
          '#plupload_settings' => array(
            'runtimes' => 'html5,flash', // Also consider the order of technologies.
            'chunk_size' => '10mb',
            'unique_names' => TRUE,
            'flash_swf_url' => file_create_url(_plupload_library_path() . '/js/plupload.flash.swf'),
            'multiple_queues' => TRUE,
          ),
          '#access' => !$form_disabled
        );
      }

    }


    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Approve'),
    );
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), $_SERVER['HTTP_REFERER'], array(
        'attributes' => array('id' => 'approve-popup-cancel-link')
      )),
    );
    $form['roles'] = array(
      '#type' => 'hidden',
      '#value' => $roles,
      '#access' => FALSE,
    );

    $form['template_title'] = array(
      '#type' => 'hidden',
      '#value' => $template_list[0]->template_title,
      '#access' => FALSE,
    );
    $form['#attached']['js'][] = drupal_get_path('module', 'open_journal').'/js/open_journal_status_sidebar_approve_popup.js'; 


  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'approve',
      '#attributes' => array('class' => array('open_journal_approve_bottom')),
    );

  }

	$form['next_status'] = array(
		'#type' => 'hidden',
		'#value' => $next_status,
		'#access' => FALSE,
  );
  $form['journal'] = array(
    '#type' => 'hidden',
    '#value' => $journal,
    '#access' => FALSE,
  );

	
	return $form;
}

function open_journal_approve_sidebar_form_validate($element, &$form_state) {
  $next_status = $element['next_status']['#value'];
  $final_url = $element['journal']['#value']->dc_identifier;

  // Todo rit remove hardcode 
  if($next_status == open_journal_get_last_state()) {
    if(!$final_url) {
      form_error($element, t('Please fill in final URL before.'));

    }
    
  }
}

  
function open_journal_approve_sidebar_form_submit($form, &$form_state) {
  $next_status = $form['next_status']['#value'];

  $jid = $form_state['build_info']['args'][0]->jid;
  $approve_reason = $form['approve_reason']['#value'];
  $template_title = $form_state['values']['template_title'];
  $roles = $form_state['values']['roles'];

  $redirect = TRUE;
  if (open_journal_get_last_state() == $next_status) {
    $redirect = FALSE;
  }
  if (isset($form_state['values']['fid'])) {
    $fid = $form_state['values']['fid'];
  }
  if (isset($form_state['values']['files_upload'])) {
    $files_upload = $form_state['values']['files_upload'];
  }

  open_journal_approve_submit($jid, $next_status, $approve_reason, $template_title, $roles, $redirect, TRUE, FALSE, $fid, $files_upload);

}

function open_journal_approve_submit($jid, $next_status, $approve_reason=NULL, $template_title=NULL, $roles=array(), $redirect=TRUE, $commit=TRUE, $silent=FALSE, $fid=FALSE, $files_upload=FALSE) {
  // TODO use $form_state['value'] for block hacker
 	//$sql = "UPDATE open_journal_journal SET status = ".$next_status." staWHERE jid = ".$jid;
	//db_query($sql);
	$journal = open_journal_journal_load($jid, TRUE);
	$journal->sid = $next_status;
	$journal = (object) $journal;
	open_journal_journal_save($journal, TRUE);
  
  db_update('open_journal_journal')
  ->fields(array('approve_by_timer' => 0))
  ->condition('jid', $journal->jid)
  ->execute();

  // Create discussion when have approve reason
  if ($approve_reason) {
    $discussion = array(
      'jid' => $jid,
      'title' => $template_title,
      'description' => $approve_reason,
    );
    if ($fid) {
      $discussion['fid'] = $fid;
    }
    if ($files_upload) {
      $discussion['files_upload'] = $files_upload;
    }
    $discussion['roles'] = $roles; 
    $discussion = (object) $discussion;
    open_journal_discussion_save($discussion);

  }

  $result_next = db_query("SELECT next_ssid from {open_journal_state} WHERE sid = :sid", array(":sid" => $journal->sid))->fetchObject();
  $bypass = FALSE;

  // TODO: check bypass condition here
  if ($result_next->next_ssid) {
    $bypass = TRUE;
  }

  if ($bypass) {
    open_journal_approve_submit($jid, $result_next->next_ssid, NULL, NULL, array(), $redirect, $commit, $silent);
  }

  if (FALSE && $redirect) {
    if (user_access('invite people to journal')) {
      drupal_set_message(t('Please invite your contributors.'));
      drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/people');
    }
    else {
      drupal_set_message(t('Approve complete.'));
      drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/overview');
    }
  }
  else {
    drupal_set_message(t('Approve complete.'));
  }


}

function open_journal_informal_approve_sidebar_form($form, &$form_state, $journal, $informal) {
  
  $status_list = unserialize(OPEN_JOUNAL_INFORMAL_STATUS);
  $form['jid'] = array(
    '#type' => 'hidden',
		'#value' => $journal->jid,
		'#access' => FALSE,
  );
    
  if ($informal->informal_approve_status == 0) {
    $form['reviewing'] = array(
  		'#markup' => '<span class="active-status">'.$status_list[0].'</span>',
  		'#weight' => -10,
  	);
  }
  else {
    $form['reviewing'] = array(
  		'#type' => 'submit',
  		'#value' => $status_list[0],
  	);
  }

	if ($informal->informal_approve_status == 1) {
  	$form['agree'] = array(
  		'#markup' => '<span class="active-status">'.$status_list[1].'</span>',
  		'#weight' => -10,
  	);
  }
  else {
  	$form['agree'] = array(
  		'#type' => 'submit',
  		'#value' => $status_list[1],
  	);
  }
  
  if ($informal->informal_approve_status == 2) {  
  	$form['fail'] = array(
  		'#markup' => '<span class="active-status">'.$status_list[2].'</span>',
  		'#weight' => -10,
  	);
  }
  else {  
  	$form['fail'] = array(
  		'#type' => 'submit',
  		'#value' => $status_list[2],
  	);
  }

  
  return $form;
  
}

function open_journal_informal_approve_sidebar_form_submit($form, &$form_state) {
  
  global $user;
  
  
  $data = $form_state['values'];
  $op = $data['op'];
  $status_list = unserialize(OPEN_JOUNAL_INFORMAL_STATUS);
  $status = array_search($op, $status_list);
  
  $sql = "SELECT MAX(created) AS created
    FROM {open_journal_journal_people} 
    WHERE uid = :uid AND jid = :jid 
    ORDER BY uid"; 

  $result = db_query($sql, array(':uid' => $user->uid, ':jid' => $data['jid']));
  $result = $result->fetchAll();
  $max_created = $result[0]->created;

  $sql = "UPDATE {open_journal_journal_people} 
    SET informal_approve_status = :status 
    WHERE jid = :jid AND uid = :uid AND created = :max_created" ;

  db_query($sql, array(':jid' => $data['jid'], ':uid' => $user->uid, ':status' => $status, ':max_created' => $max_created));

  /*
  db_update('open_journal_journal_people')
    ->fields(array(
      'informal_approve_status' => $status
    ))
    ->condition('jid', $data['jid'], '=')
    ->condition('uid', $user->uid, '=')
    ->execute();

    */
    
  
}

function open_journal_update_state($journal) {

  // TODO: check state can approve by author or remove this url and trigger by upload file

  $next_sid= open_journal_next_sid($journal->sid, TRUE);
  open_journal_approve_submit($journal->jid, $next_sid, NULL, NULL, array(), FALSE);
  return 'open_journal_update_state';
}

function open_journal_approve_by_timer($journal) {
  db_update('open_journal_journal')
    ->fields(array('approve_by_timer' => 1))
    ->condition('jid', $journal->jid)
    ->execute();

  drupal_set_message(t('Your postpone has been approved'));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid .'/discussion');

  return 'open_journal_approve_by_timer';
}

function open_journal_reject_state($journal) {
  dpm($journal);
  $reject_reason = '';
  open_journal_reject_submit($journal->jid, $reject_reason, FALSE, FALSE, FALSE);
  return 'open_journal_reject_state';
}

function open_journal_check_timeout_exist($journal, $state, $type, $now=NULL) {

  if (!$state->{$type}) {
    return FALSE;
  }

  $now = $now? $now: time();
  $seconds_per_day = variable_get('seconds_per_day', 60*60*24);

  $days_after_status_changed = (($now - $journal->status_changed)/$seconds_per_day);

  $exist = db_query("SELECT * FROM {open_journal_timeout} WHERE jid = :jid AND sid = :sid AND type = :type", array(
    ':jid' => $journal->jid,
    ':sid' => $state->sid,
    ':type' => $type
  ))->rowCount();


  if ($days_after_status_changed >= $state->{$type} && !$exist) {
    db_insert('open_journal_timeout')
      ->fields(array('jid' => $journal->jid, 'sid' => $state->sid, 'type' => $type))
      ->execute();

    return TRUE;
  }
  elseif ($days_after_status_changed < $state->{$type} && $type == 'timeout_auto_approve') {
    $next_sid = open_journal_next_sid($state->sid);
    $next_state = db_query("SELECT name FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $next_sid))->fetchObject();

    drupal_set_message('Article ID: <strong>'.l($journal->code, OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid).'</strong> will change state from <strong>'.$state->name.'</strong> to <strong>'.$next_state->name.'</strong> in '.floor($state->{$type} - $days_after_status_changed).' days');
  }

  return FALSE;
}

function open_journal_timeout_check() {
  $now = time();
  $sql = "SELECT * FROM {open_journal_state} WHERE timeout_auto_reminder > 0 OR timeout_auto_approve > 0 OR timeout_auto_reject > 0";

  $state_list = db_query($sql)->fetchAll();
  foreach ($state_list as $state) {
      $sql = "SELECT * FROM {open_journal_journal} WHERE sid = :sid AND reject = 0 AND withdraw = 0";
      $journal_list = db_query($sql, array(':sid' => $state->sid))->fetchAll();

      foreach ($journal_list as $journal) {

        if (OPEN_JOURNAL_SUB_LANG_ENABLE && $journal->dc_language != 'en') {
          $journal->title = $journal->title_sub? $journal->title_sub: $journal->title_en;
        }
        else {
          $journal->title = $journal->title_en? $journal->title_en: $journal->title_sub;
        }

        if (open_journal_check_timeout_exist($journal, $state, 'timeout_auto_reminder', $now)) {
          drupal_set_message('timeout_auto_reminder : '.$journal->jid);
          open_journal_discussion_when_state_change($journal, 'time', FALSE);
        }
        else if (open_journal_check_timeout_exist($journal, $state, 'timeout_auto_approve', $now) && $journal->approve_by_timer) {
          drupal_set_message('timeout_auto_approve : '.$journal->jid);
          $next_sid= open_journal_next_sid($journal->sid);
          open_journal_approve_submit($journal->jid, $next_sid, NULL, NULL, array(), FALSE);
        }
        else if (open_journal_check_timeout_exist($journal, $state, 'timeout_auto_reject', $now)) {
          $reject_reason = 'Timeout';
          db_update('open_journal_journal')
            ->fields(array('reject' => 1, 'reject_reason' => $reject_reason))
            ->condition('jid', $journal->jid)
            ->execute();

          open_journal_discussion_when_state_change($journal, 'reject', FALSE);
          drupal_set_message('timeout_auto_reject : '.$journal->jid);
        } 
      }


  }

  return '';

}
