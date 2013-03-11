<div class="journal-block discussion-block block-bg-shadow">
    <h2 class="journal-block-title">Discussion</h2>
  
    <h3><?php print $discussion->title; ?></h3>
  
    <ul class="discussion-list">
        <?php print theme('open_journal_item', array(
            'item' => $discussion,
            'journal' => $journal
        )); ?>
    </ul>
  
    <div class="journal-comment-form-wrapper">
        <?php print $current_user_info['display_picture']; ?>
        <div class="discussion-item-content">
            <div class="journal-comment-form-submited">
                <?php print $current_user_info['display_name']; ?> - <?php print date('d M Y'); ?>
            </div>
            
            <?php print $comment_form; ?>
        </div>
    </div>
  
</div>