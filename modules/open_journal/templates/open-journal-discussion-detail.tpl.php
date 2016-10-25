<div class="journal-block discussion-block block-bg-shadow">
    <h2 class="journal-block-title">Discussion</h2>
    
    <?php if ($discussion->contributor_only): ?>
    <span class="contributor-only">contributor only.</span>
    <?php endif ?>
  
    <h3><?php print $discussion->title; ?></h3>
  
    <ul class="discussion-list">
        <?php print theme('open_journal_discussion_item', array(
            'item' => $discussion,
            'journal' => $journal
        )); ?>
        
        <?php foreach ($comment_list as $comment): ?>
        <?php print theme('open_journal_comment_item', array(
          'item' => $comment,
          'journal' => $journal,
          'discussion' => $discussion,
        )); ?>
        <?php endforeach ?>
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