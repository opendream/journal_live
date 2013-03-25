<div class = "open-journal-register">
  <?php print $form;?>
  <div id="member-condition">
  	<h3>Member guideline</h3>
		<?php foreach($condition_list as $condition):?>
		<div> - <?php print $condition;?></div>
		<?php endforeach;?>
  </div>
</div>