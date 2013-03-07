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
		<?php foreach ($journal_list as $journal): ?>
			<li class="journal-item"><?php print theme('open_journal_list_item', array('journal' => $journal)); ?><li>
		<?php endforeach ?>
	</ul>
	<div class="load-more">
		<?php print l('Load More ...', url(OPEN_JOURNAL_PREFIX_PATH.'/ajax/journal/list/'.$journal->jid.'/'.$limit) ,array(
			'attributes' => array(
				'id' => 'journal-list-load-more',
		 	))
		);
		?>
	</div>

</div>
