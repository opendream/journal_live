<div class="journal-block discussion-block block-bg-shadow">
  <h2 class="journal-block-title">Discussion</h2>
  
  <h3><?php print $discussion->title; ?></h3>
  
  <ul class="discussion-list">
    <?php print theme('open_journal_item', array(
        'item' => $discussion,
        'journal' => $journal
    )); ?>
  </ul>
  
</div>