<div class="sidebar-status-block block-bg-shadow">
	<h2 class="journal-block-title">Status</h2>
	<?php foreach($status_list as $key_status => $value_status):?>
	<div 
	<?php if($key_status < $journal->status):print " class='status-active' ";
	elseif($key_status == (count($status_list)-1) && $key_status == $journal->status):print " class='status-active finish' ";
	elseif($key_status == $journal->status):print " class='status-active current' ";
	else:print " class='status-disabled' ";endif;?>>
	
		<div class="status-number discussion-sidebar-status-<?php print $key_status;?>" >
		<?php print $key_status+1;?>
		</div>
    
    <div class="status-text<?php print $status_add_class; ?>">
		  <?php print $value_status;?>
		</div>

		
		<?php if ($approve_form): ?>
		<?php if($journal->status == $key_status && $journal->status != count($status_list)-1 && !$journal->reject):?>
			<?php print $approve_form;?>
		<?php endif;?>
		<?php endif ?>
	
	</div>

	<?php endforeach;?>
	
	<?php if ($reject_form): ?>
	<?php print $reject_form;?>
	<div class="reject-button">
		<?php print 'or '.l('Rejecte' ,'', array('attributes' => array('id' => 'reject_bottom')));?>
	</div>
	<?php endif ?>
	
	<?php if ($journal->reject): ?>
  <div class="reject-tag">
    Rejected
  </div>
	<?php endif ?>

</div>