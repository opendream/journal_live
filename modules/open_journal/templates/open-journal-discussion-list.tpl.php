<div class="journal-discussion-list-block block-bg-shadow">
	<div class="add-discussion-btn">
		<?php print l(t('Start new discussion'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/add'); ?>
	</div>
	<?php foreach($discussion_list as $discussion_item):?>
	    <div class="journal-discussion-list-item">
	      
	      <?php if ($discussion_item->contributor_only): ?>
        <span class="contributor-only-icon">contributor only</span>
        <?php endif ?>
	      
	      <div class="journal-user">
	        <?php print $discussion_item->user['display_picture'];?>
	        <?php print $discussion_item->user['display_name'];?>
	      </div>
	      
	      <div class="discus-title">
	      	
	      	<?php if($discussion_item->fid):?>
	      		<?php  print l($discussion_item->title.' <span class="file-attach">'.$image_file.'</span>', OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/'.$discussion_item->did, array('html' => TRUE));?>
	      	<?php else:?>
	      		<?php  print l($discussion_item->title, OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/'.$discussion_item->did, array('html' => TRUE));?>
	      	<?php endif;?>
	      		
	      </div>
	      <div class="discus-content"><?php print $discussion_item->description;?></div>
	    </div>
	<?php endforeach;?>
</div>