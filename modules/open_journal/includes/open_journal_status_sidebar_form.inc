<?php
function open_journal_reject_sidebar_form($form, &$form_state, $journal) {
	
	
	$form['reject_reason'] = array(
	  '#title' => t('Write your reject reason.'),
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
	global $user;
	$reject_reason = $form['reject_reason']['#value'];
	$jid = $form_state['build_info']['args'][0]->jid;
	$sql = "UPDATE open_journal_journal SET reject = 1, reject_reason = '".$reject_reason."' WHERE jid = ".$jid;
	db_query($sql);
	drupal_set_message('This journal is rejected ');


	// Create new discustion
	/*$discussion = array(
      'uid' => $user->uid,
      'jid' => $jid,
      'title' => t('This journal has been rejected.'),
      'description' => 'You can view reject reason this link.'."\n".url('journal/'.$data->jid.'/discussion', array('absolute' => TRUE)),
      'contributor_only' => 0,
      'created' => mktime(0, 0, 0, date('m'), date('d'), date('Y')),
      'permanent' => 1
    );

    $discussion = (object) $discussion;
    open_journal_discussion_save($discussion);
    */
	
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
	drupal_set_message('This journal is approve ');
	// TODO send mail 

}