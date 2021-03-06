<li class="comment-item" id="comment-item-<?php print $item->cid; ?>">
    <?php print $item->user_info['display_picture']; ?>
    <div class="comment-item-content">
        
        <div class="discussion-item-submited">
            <?php print $item->user_info['display_name']; ?> - <?php print date('d M Y', $item->created); ?>
        </div>        

        <?php if ($item->can_edit): ?>
        <div class="comment-item-edit-link">
            <?php print l('Edit', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/edit', array('attributes' => array('class' => array('open-journal-ajax-edit-link')))); ?>
             · 
            <?php print l('Delete', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/delete', array('attributes' => array('class' => array('open-journal-ajax-delete-link')))); ?>
        </div>
        <?php endif ?>
        
        <div class="comment-item-message">
            <?php print $item->message; ?>
        </div> 
        
        <?php if ($item->fid && $item->file): ?>
        <div class="comment-item-file">
            <?php print l($item->file->filename, file_create_url($item->file->uri));?>
        </div>
        <?php endif ?>
        
    </div>

</li>