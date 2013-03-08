
<li class="journal-item">

	<div class="journal-status status<?php echo $journal->status;?>"><span><?php print $journal->status_name;?></span></div>
	<h3 class="journal-title"><?php print l($journal->dc_title, 'journal/'.$journal->jid.'/overview');?></h3>
	<div class="journal-update">
		<span class="label">Status update: </span>
		<span><?php print date('d M Y', $journal->status_changed);?></span>
	</div>
	<div class="journal-create">
		<span class="label">Create Date: </span>
		<span><?php print date('d M Y', $journal->created);?></span>		
	</div>
	<ul class="relate-peoples">
		<?php foreach ($journal->people_list as $key_people => $people):?>
		<?php if($key_people == 'more'):?>
		<li class="more-people">
		  <?php $people->display_info['display_picture']; ?>
		</li>
		<?php else:?>
		<li class="clickable-people profile-picture-small">
			<?php print $people->display_info['display_picture']; ?>
		</li>
		<?php endif;?>
		<?php endforeach;?>
	</ul>
</li>

