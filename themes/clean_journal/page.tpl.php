
  <?php if(!empty($open_journal_logo2)):?>
    <div id="open-journal-sign-logo">
      <?php print l($open_journal_logo2, '', array('html' => TRUE));?>
    </div>
  <?php endif;?>
  
  <?php if (isset($journal) && !$disabled_title): ?>
    <h1 class="page-title"><?php print $journal->title; ?></h1>
  <?php elseif ($title && !$disabled_title): ?>
    <h1 class="page-title"><?php print $title; ?></h1>
  <?php endif; ?>

  <?php print $outer_header; ?>

  <div id="page" class="<?php print $page_class; ?>">
    <div id="content" class="clearfix">
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </div>

  </div>
