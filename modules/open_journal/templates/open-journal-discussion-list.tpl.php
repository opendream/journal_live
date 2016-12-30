
<div class="journal-discussion-list-block block-bg-shadow">

	<div class="latest-file-link">
		<img typeof="foaf:Image" src="/sites/all/modules/open_journal/images/ic-has-file.png" alt="has file attach">
		<?php print l(t('Latest file'), file_create_url($file->uri)); ?>
		by <?php print $file->user_info['display_name']; ?>
	</div>

	<div class="add-discussion-btn">
		<?php print l(t('Start new discussion'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/add'); ?>
	</div>
	<?php foreach($discussion_list as $discussion_item):?>
	    <div class="journal-discussion-list-item<?php if ($discussion_item->auto): ?> auto<?php else: ?> human<?php endif; ?>">
	      
	      <?php if ($discussion_item->contributor_only): ?>
        <span class="contributor-only-icon">contributor only</span>
        <?php endif ?>
	      
	      <div class="journal-user">
	        <?php print $discussion_item->user['display_picture'];?>
	        <?php print $discussion_item->user['display_name'];?>
	      </div>
	      
	      <div class="discus-title">
	      	<?php print $discussion_item->has_files_upload; ?> zz
	      	<?php if($discussion_item->auto):?>
	      		<?php  print $discussion_item->title; ?>
	      	<?php elseif($discussion_item->fid || $discussion_item->has_files_upload > 0):?>
	      		<?php  print l($discussion_item->title.' <span class="file-attach">'.$image_file.'</span>', OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/'.$discussion_item->did, array('html' => TRUE));?>
	      	<?php else:?>
	      		<?php  print l($discussion_item->title, OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/'.$discussion_item->did, array('html' => TRUE));?>
	      	<?php endif;?>
	      		
	      </div>
	      <div class="discus-content"><?php print $discussion_item->description;?></div>
	    </div>
	<?php endforeach;?>
</div>
