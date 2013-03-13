<?php
function open_journal_reject_sidebar_form($form, &$form_state, $journal) {
	
	$form['reject_reason'] = array(
		'#type' => 'textarea',
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

function open_journal_reject_sidebar_form_submit($form, &$form_state) {
	
	$reject_reason = $form['reject_reason']['#value'];
	$jid = $form_state['build_info']['args'][0]->jid;
	$sql = "UPDATE open_journal_journal SET reject = 1, reject_reason = '".$reject_reason."' WHERE jid = ".$jid;
	db_query($sql);

	// Create new discustion
	$reject_reason = open_journal_trim($reject_reason)? $reject_reason:'We are sorry. If you want to more information to rejected, you can talk on this discussion.' ;
	$discussion = array(
    'jid' => $jid,
    'title' => t('This journal has been rejected.'),
    'description' => $reject_reason,
    
  );

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  
  drupal_set_message('This journal is rejected.');
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/discussion/'.$discussion->did);
	
	
}


function open_journal_approve_sidebar_form($form, &$form_state, $journal , $next_status) {
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'approve',
		'#attributes' => array('class' => array('open_journal_approve_bottom')),
	);
	$form['next_status'] = array(
		'#type' => 'hidden',
		'#value' => $next_status,
		'#access' => FALSE,

		);

	
	return $form;
}

function open_journal_approve_sidebar_form_submit($form, &$form_state) {
	$next_status = $form['next_status']['#value'];
	$jid = $form_state['build_info']['args'][0]->jid;
	$sql = "UPDATE open_journal_journal SET status = ".$next_status." WHERE jid = ".$jid;
	db_query($sql);
	
	// Create new discustion
	$status_list = unserialize(OPEN_JOUNAL_STATUS);
	$status_name = $status_list[$next_status];

	$discussion = array(
    'jid' => $jid,
    'title' => t('Congratulation! Status changed to '.$status_name ),
    'description' => 'Talk on discussion if you have some question?',
  );

  $discussion = (object) $discussion;
  open_journal_discussion_save($discussion);
  
  drupal_set_message(t('Please, invite contributors to join on this journal.'));
  drupal_goto(OPEN_JOURNAL_PREFIX_PATH.'/'.$jid .'/people');

}

function open_journal_informal_approve_sidebar_form($form, &$form_state, $journal , $next_status) {
}