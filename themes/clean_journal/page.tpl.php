
  <?php if(!empty($open_journal_logo2)):?>
    <div id="open-journal-sign-logo">
      <?php if ($logo): ?>
      <?php print l(theme('image', array('path' => $logo)), '', array('html' => TRUE));?>
      <?php else: ?>
      <?php print l($open_journal_logo2, '', array('html' => TRUE));?>  
      <?php endif ?>
    </div>
  <?php endif;?>
  
  <?php if (isset($journal) && !$disabled_title): ?>
    <h1 class="page-title">
      <?php if(isset($journal->title)):?>
      <?php if ($journal->code): ?>
      <?php print $journal->code; ?> -
      <?php endif ?>
      <?php print $journal->title; ?>
      <?php endif;?>
    </h1>
  <?php elseif ($title && !$disabled_title): ?>
    <h1 class="page-title"><?php print $title; ?></h1>
  <?php endif; ?>

  <?php print $outer_header; ?>

  <div id="page" class="<?php print $page_class; ?>">
    <div id="content" class="clearfix">
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      
      <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        
      <?php print render($page['content']); ?>
    </div>

  </div>

  
