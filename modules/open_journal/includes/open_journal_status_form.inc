<?php
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

function open_journal_reject_submit($jid, $reject_reason='', $redirect=TRUE) {
  $sql = "UPDATE open_journal_journal SET reject = 1, reject_reason = '".$reject_reason."' WHERE jid = ".$jid;
  db_query($sql);

  // Create new discustion
  $reject_reason = open_journal_trim($reject_reason)? $reject_reason:'We are sorry. If you want to more information to rejected, you can talk on this discussion.' ;
  $discussion = array(
    'jid' => $jid,
    'title' => t('This journal has been rejected.'),
    'description' => $reject_reason,
    
  );

  if (isset($form_state['values']['roles'])) {
    $discussion['roles'] = $form_state['values']['roles'];
  }

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  
  if ($redirect) {
    drupal_set_message('This journal is rejected.');
    drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/discussion/'.$discussion->did);
  }
  
}

function open_journal_reject_sidebar_form_submit($form, &$form_state) {
	$reject_reason = $form['reject_reason']['#value'];
	$jid = $form_state['build_info']['args'][0]->jid;

  open_journal_reject_submit($jid, $reject_reason);
	
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


function open_journal_approve_sidebar_form($form, &$form_state, $journal , $next_status, $template_list=FALSE) {

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'open_journal') . '/js/open_journal_status_sidebar.js',
    drupal_get_path('module', 'open_journal') . '/js/jquery.qtip.min.js',
  );
  if($template_list) {
    $default_value = $template_list[0]->body;
    $roles = array();
    foreach ($template_list[0]->role as $key => $value) {
      $roles[$value->rid] = $value->rid;
    }
    $form['approve_reason'] = array(
      '#type' => 'textarea',
      '#default_value' => $default_value,
    );

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
  if($next_status == 2) {
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
  open_journal_approve_submit($jid, $next_status, $approve_reason, $template_title, $roles);

}

function open_journal_approve_submit($jid, $next_status, $approve_reason=NULL, $template_title=NULL, $roles=array(), $redirect=TRUE, $commit=TRUE, $silent=FALSE) {

  // TODO use $form_state['value'] for block hacker
 	//$sql = "UPDATE open_journal_journal SET status = ".$next_status." staWHERE jid = ".$jid;
	//db_query($sql);
	$journal = open_journal_journal_load($jid);
	$journal->sid = $next_status;
	$journal = (object) $journal;
	open_journal_journal_save($journal, TRUE);
  
  /*
	// Create new discustion
	//$status_list = unserialize(OPEN_JOUNAL_STATUS);
  $status_list = open_journal_get_status_list();
	$status_name = $status_list[$next_status];

  // Auto create discussion when stage change
  $sql = "SELECT * FROM {open_journal_template} WHERE sid = :sid AND auto = :auto AND reject = :reject"; 
  $result = db_query($sql, array(':sid' => $next_status, ':auto' => 1, ':reject' => 0));
  $template_list = $result->fetchAll();

  if (!empty($template_list)) {
    open_journal_replace_template_string($template_list, $journal);

    foreach ($template_list as $key => $value) {

      $discussion = array(
        'jid' => $jid,
        'title' => t('Congratulations! Status is now changed to '.ucwords($status_name) ),
      );
      $discussion['roles'] = array();
      $role_list = open_journal_get_template_role_list($value->tpid);

      foreach ($role_list as $key2 => $value2) {
        $discussion['roles'][] = $value2->rid;
      }
      $discussion['description'] = $value->body;

    }



  }
  else {
    $discussion = array(
      'jid' => $jid,
      'title' => t('Congratulations! Status is now changed to '.ucwords($status_name) ),
      'description' => 'Your comments are now welcome in Discussions.',
      'roles' => array(),
    );

  }

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  */

  // Create discussion when have approve reason
  if ($approve_reason) {
    $discussion = array(
      'jid' => $jid,
      'title' => $template_title,
      'description' => $approve_reason,
    );
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

  if ($redirect) {
    if (user_access('invite people to journal')) {
      drupal_set_message(t('Please invite your contributors.'));
      drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/people');
    }
    else {
      drupal_set_message(t('Approve complete.'));
      drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/overview');
    }
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
  dpm($journal);
  $next_sid= open_journal_next_sid($journal->sid);
  open_journal_approve_submit($journal->jid, $next_sid, NULL, NULL, array(), FALSE);
  return 'open_journal_update_state';
}

function open_journal_reject_state($journal) {
  dpm($journal);
  $reject_reason = '';
  open_journal_reject_submit($journal->jid, $reject_reason, FALSE);
  return 'open_journal_reject_state';
}

function open_journal_timeout_check() {
  $now = time();
  $sql = "SELECT * FROM {open_journal_state} WHERE timeout_auto_reminder > 0 OR timeout_auto_approve > 0 OR timeout_auto_reject > 0";

  $state_list = db_query($sql)->fetchAll();
  foreach ($state_list as $state) {
      $sql = "SELECT * FROM {open_journal_journal} WHERE sid = :sid AND reject = 0";
      $journal_list = db_query($sql, array(':sid' => $state->sid))->fetchAll();

      foreach ($journal_list as $journal) {
        if ($state->timeout_auto_reminder > 0) {
          dpm('timeout_auto_reminder '.$journal->jid). ' '. $journal->state_changed;
        }
        if ($state->timeout_auto_approve > 0) {
          dpm('timeout_auto_approve '.$journal->jid);
        }
        if ($state->timeout_auto_reject > 0) {
          dpm('timeout_auto_reject '.$journal->jid);
        } 
      }


  }

  return '';

  //open_journal_approve_submit($jid, $next_status, $approve_reason=NULL, $template_title=NULL, $roles=array(), $redirect=TRUE, $commit=TRUE, $silent=FALSE)
}






















