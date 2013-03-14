<div id="user-heading">
	<h1>Journals List</h1>
	<?php print $user_info['display_picture'];?>
	<h2><?php print $user_info['display_name'];?> </h2>
	<h3>(<?php print $user_roles;?>)</h3>
	<span><?php print $count_journal; ?> Journals</span>

</div>

<div class="journal-filter-tab">
	<div class="journal-filter">
		<div class="journal-filter-active"><?php print $current_status_name; ?></div>
	</div>
	<ul id="journal-filter-item-list">
		<li class="journal-filter-item"><?php print l('All Journals', OPEN_JOURNAL_PREFIX_PATH);?></li>
		<?php foreach($status_list as $key_status => $status):?>
		<li class="journal-filter-item"><?php print l($status, OPEN_JOURNAL_PREFIX_PATH.'/list/'.$key_status);?></li>
		<?php endforeach;?>
		
	</ul>
	<?php print $not_reject_form;?>
	<div class="add-journal-btn">
		<?php print l(t('Add New'), OPEN_JOURNAL_PREFIX_PATH.'/add'); ?>
	</div>
</div>

<?php if ($count_journal): ?>
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
<?php else: ?>
<div class="journal-list-empty block-bg-shadow">
  <h2>Create Your First Journal</h2>
  <div class="add-journal-btn">
		<a href="/journal/add">Add New</a>
	</div>
</div>
<?php endif ?>
