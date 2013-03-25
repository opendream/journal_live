
<li class="journal-item">

	
	
	<?php print l("<div class='journal-status status".$journal->status_class."'> <span>".$journal->status_name."</span> </div>", OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/overview', array('html' => TRUE));?>

	<h3 class="journal-title"><?php print l($journal->dc_title, OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/overview');?></h3>
	<div class="journal-update">
		<span class="label">Status update: </span>
		<span><?php print date('d M Y', $journal->status_changed);?></span>
	</div>
	<div class="journal-create">
		<span class="label">Creation Date: </span>
		<span><?php print date('d M Y', $journal->created);?></span>		
	</div>
	<ul class="relate-peoples">
		<?php foreach ($journal->people_list as $key_people => $people):?>
		<?php if($key_people == 'more'):?>
		<li class="more-people">
		  <?php print $people->display_info['display_picture']; ?>
		</li>
		<?php else:?>
		<li class="clickable-people profile-picture-small">
			<?php print $people->display_info['display_picture']; ?>
		</li>
		<?php endif;?>
		<?php endforeach;?>
	</ul>
</li>

