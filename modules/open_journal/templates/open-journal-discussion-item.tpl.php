<li class="discussion-item">
    <?php print $item->user_info['display_picture']; ?>
    <div class="discussion-item-content">
        <div class="discussion-item-submited">
            <?php print $item->user_info['display_name']; ?> - <?php print date('d M Y', $item->created); ?>
        </div>        

        <?php if ($item->can_edit): ?>
        <div class="discussion-item-edit-link">
            <?php print l('Edit', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$item->did.'/edit', array('attributes' => array('class' => array('open-journal-ajax-edit-link')))); ?>
            <?php if (!$item->permanent): ?>
            <?php print l('Delete', OPEN_JOURNAL_PREFIX_PATH.'/'.$journal->jid.'/discussion/'.$item->did.'/delete', array('attributes' => array('class' => array('open-journal-ajax-delete-link')))); ?>
            <?php endif ?>
        </div>
        <?php endif ?>
        
        <?php if ($item->description): ?>
        <div class="discussion-item-description">
            <?php print $item->description; ?>
        </div> 
        <?php endif ?>
        
        <?php if ($item->fid && $item->file): ?>
        <div class="discussion-item-file">
            <?php print l($item->file->filename, file_create_url($item->file->uri)); ?>
        </div>
        <?php endif ?>

    </div>

</li>
