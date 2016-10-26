<div class="sidebar-status-block block-bg-shadow">
	<h2 class="journal-block-title">Status</h2>
	<?php 
		$num_root = 0;
		$num_child = 0;
		//dpm(open_journal_next_sid(2));
	?>
	<?php foreach($status_list as $key_status => $value_status):?>
	<div 
	<?php 
	if($value_status['active']): print " class='status-active' ";
	elseif($key_status == (count($status_list)-1) && $value_status['active']):print " class='status-active finish' ";
	elseif($value_status['current']):print " class='status-active current' ";
	else:print " class='status-disabled' ";endif;?>>

		<?php 
			if ($value_status['parent_sid']) {
				$num_child++;
				$status_label = $num_root.".".$num_child;
				$status_extra_class = "child";
			}
			else {
				$num_root++;
				$num_child = 0;
				$status_label = $num_root;
				$status_extra_class = "parent";
			}

		?>
	
		<div class="status-number discussion-sidebar-status-<?php print $key_status;?> <?php print $status_extra_class;?>" >
		<?php
			print $status_label;
		?>
		</div>
    
    <div class="status-text<?php print $status_add_class; ?>">
		  <?php print $value_status['name'];?>
		</div>

		
		<?php if ($approve_form): ?>
		<?php if($journal->sid == $value_status['sid'] && $value_status['next_sid'] != NULL && !$journal->reject):?>
			<?php print $approve_form;?>
			<?php if($approve_template):?>
				<div class="approve-popup-button">
					<?php print l('Approve' ,'', array('attributes' => array('id' => 'approve_popup_bottom')));?>
				</div>

			<?php endif;?>
		<?php endif;?>
		<?php endif ?>
	
	</div>

	<?php endforeach;?>
	
	<?php if ($reject_form): ?>
	<?php print $reject_form;?>
	<div class="reject-button">
		<?php print 'or '.l('Reject' ,'', array('attributes' => array('id' => 'reject_bottom')));?>
	</div>
	<?php endif ?>
	
	<?php if ($journal->reject): ?>
  <div class="reject-tag">
    Rejected
  </div>
	<?php endif ?>

</div>