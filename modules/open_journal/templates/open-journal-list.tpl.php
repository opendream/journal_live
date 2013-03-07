<div id="user-heading">
	<h1>Journals List</h1>
	<?php print $journal_list['user_info']['display_picture'];?>
	<h2><?php print $journal_list['user_info']['display_name'];?></h2>
	<span>4 Journals</span>
</div>

<div class="journal-filter-tab">
	<div class="journal-filter">
		<div class="journal-filter-active">All Journals</div>
	</div>
	<ul id="journal-filter-item-list">
		<li class="journal-filter-item select-item"><a href="#">All Journals</a></li>
		<li class="journal-filter-item"><a href="#">Option 1</a></li>
		<li class="journal-filter-item"><a href="#">Option 2</a></li>
		<li class="journal-filter-item"><a href="#">Option 3</a></li>
		<li class="journal-filter-item"><a href="#">Option 4</a></li>
	</ul>
	<div class="add-journal-btn">
		<?php print l(t('Add New'), OPEN_JOURNAL_PREFIX_PATH.'/add'); ?>
	</div>
</div>

<div id="journal-list">
	<ul>
		<?php foreach ($journal_list['list'] as $value):?>
		<li class="journal-item">
			<div class="journal-status status<?php echo $value->status;?>"><span><?php print $value->status_name;?></span></div>
			<h3 class="journal-title"><a href="/journal/1/overview"><?php print $value->dc_title;?></a></h3>
			<div class="journal-update">
				<span class="label">Status update: </span>
				<span><?php print date('d M Y', $value->status_changed);?></span>
			</div>
			<div class="journal-create">
				<span class="label">Create Date: </span>
				<span><?php print date('d M Y', $value->created);?></span>		
			</div>
			<ul class="relate-peoples">
				<?php foreach ($value->list_people as $key_info => $value_info):?>
				<?php if($key_info == 'more'):?>
				<li class="more-people"><?php print l($value_info['display_picture'], '');?></li>
				<?php elseif($value_info->uid == $value->uid):?>
				<li class="clickable-people owner profile-picture-small">

					<?php print l($value_info->user_info['display_picture'].'<span>Owner</span>', '' ,array('html' => TRUE))?>
				<?php else:?>
				<li class="clickable-people  profile-picture-small">
					<?php print l($value_info->user_info['display_picture'], '' ,array('html' => TRUE))?>
				<?php endif;?>
				</li>
				<?php endforeach;?>
				
			</ul>
		</li>
		<?php endforeach;?>	
	</ul>
	<div class="load-more"><a href="#">Load More …</a></div>
</div>