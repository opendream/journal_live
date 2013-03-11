<li class="comment-item">
    <?php print $item->user_info['display_picture']; ?>
    <div class="comment-item-content">
        <div class="comment-item-submited">
            <?php print $item->user_info['display_name']; ?> - <?php print date('d M Y', $item->created); ?>
        </div>
        
        <div class="comment-item-message">
            <?php print $item->message; ?>
        </div> 
        
        <?php if ($item->fid): ?>
        <div class="comment-item-file">
            <?php print $item->file->filename; ?>
        </div>
        <?php endif ?>
        
        <?php if ($item->can_edit): ?>
        <div class="comment-item-edit-link">
            <?php print l('Edit', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/edit'); ?>
             · 
            <?php print l('Delete', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/delete'); ?>
        </div>
        <?php endif ?>

    </div>

</li>