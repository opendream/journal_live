<li class="comment-item">
    <?php print $item->user_info['display_picture']; ?>
    <div class="comment-item-content">
        
        <div class="discussion-item-submited">
            <?php print $item->user_info['display_name']; ?> - <?php print date('d M Y', $item->created); ?>
        </div>        
        
        <div class="comment-item-message">
            <?php print $item->message; ?>
        </div> 
        
        <?php if ($item->fid && $item->file): ?>
        <div class="comment-item-file">
            <a href="#"><?php print $item->file->filename; ?></a>
        </div>
        <?php endif ?>
        
        <?php if ($item->can_edit): ?>
        <div class="comment-item-edit-link">
            <?php print l('Edit', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/edit', array('attributes' => array('class' => array('open-journal-ajax-edit-link')))); ?>
             · 
            <?php print l('Delete', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$discussion->did.'/comment/'.$item->cid.'/delete', array('attributes' => array('class' => array('open-journal-ajax-delete-link')))); ?>
        </div>
        <?php endif ?>

    </div>

</li>