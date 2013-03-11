<li class="discussion-item">
    <?php print $item->user_info['display_picture']; ?>
    <div class="discussion-item-content">
        <div class="discussion-item-submited">
            <?php print $item->user_info['display_name']; ?> - <?php print date('d M Y', $item->created); ?>
        </div>
        
        <?php if ($item->description): ?>
        <div class="discussion-item-description">
            <?php print $item->description; ?>
        </div>    
        <?php endif ?>
        
        <?php if ($item->fid): ?>
        <div class="discussion-item-file">
            <?php print $item->file->filename; ?>
        </div>
        <?php endif ?>
        
        <?php if ($item->can_edit): ?>
        <div class="discussion-item-edit-link">
            <?php print l('Edit', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/'.$item->type.'/'.$item->id.'/edit'); ?>
             · 
            <?php print l('Delete', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/'.$item->type.'/'.$item->id.'/delete'); ?>
        </div>
        <?php endif ?>

    </div>

</li>