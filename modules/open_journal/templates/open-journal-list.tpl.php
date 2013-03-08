<div id="user-heading">
	<h1>Journals List</h1>
	<?php print $user_info['display_picture'];?>
	<h2><?php print $user_info['display_name'];?></h2>
	<span><?php print $count_journal; ?> Journals</span>
</div>

<div class="journal-filter-tab">
	<div class="journal-filter">
		<div class="journal-filter-active">All Journals</div>
	</div>
	<ul id="journal-filter-item-list">
		<li class="journal-filter-item select-item"><a href="#">All Journals</a></li>
		<?php foreach($status_list as $key_status => $status):?>
		<li class="journal-filter-item"><?php print l($status, '');?></li>
		<?php endforeach;?>
		
	</ul>
	<div class="add-journal-btn">
		<?php print l(t('Add New'), OPEN_JOURNAL_PREFIX_PATH.'/add'); ?>
	</div>
</div>

<div id="journal-list">
	<ul id="journal-list-container">
		<?php foreach ($journal_list as $journal): ?>
			<?php print theme('open_journal_list_item', array('journal' => $journal)); ?>
		<?php endforeach ?>
	</ul>

	<?php if ($update_load_more_url): ?>
	<div class="load-more">
		<a class="load-more-ajax" rel="journal-list-container" href="<?php print $update_load_more_url; ?>">Load More ...</a>
	</div>	
	<?php endif; ?>


</div>
