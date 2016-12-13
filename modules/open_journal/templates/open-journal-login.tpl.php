<div class = "open-journal-login">
	<?php print $form;?>
	<div id="member-condition">
		<h3>Member instruction</h3>
		<?php print theme('item_list', array('items' => $condition_list)) ?>
	</div>
</div>