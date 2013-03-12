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
	$jid = $form_state['build_info']['args'][0]->jid;
	$sql = "UPDATE {open_journal_journal} SET reject = 1 WHERE jid = ".$jid;
	//db_query($sql);

	// TODO send mail * mail includeing reject reason 
	
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
	$sql = "UPDATE {open_journal_journal} SET status = ".$next_status." WHERE jid = ".$jid;
	db_query($sql);

	// TODO send mail 

}