<div>
	<h2>Status</h2>
	<?php foreach($status_list as $key_status => $value_status):?>
	
	<div>
	<?php print $key_status+1;?>
	<?php print $value_status;?>
	<?php if($journal->status == $key_status):?>
		<a href="">Approve</a>
	<?php endif;?>	
	</div>

	<?php endforeach;?>

	<div>
	or <a href="#">Reject</a>
	</div>
</div>