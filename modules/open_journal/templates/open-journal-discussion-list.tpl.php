
<div class="add-journal-btn">
	<?php print l(t('Add New'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/add'); ?>
</div>
<?php foreach($discussion_list as $discussion_item):?>
    <div class="journal-detail-content-item">
      <div><?php print $discussion_item->user['display_picture'];?> <span><?php print $discussion_item->user['display_name'];?></span> </div>
      <div><?php print $discussion_item->title;?></div>
      <div><?php print $discussion_item->description;?></div>
    </div>
<?php endforeach;?>