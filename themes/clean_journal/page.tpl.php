
  <?php 
  if(!empty($open_journal_logo2)):?>
    <div id="open-journal-sign-logo">
      <?php print l($open_journal_logo2, '', array('html' => TRUE));?>
    </div>
  <?php endif;?>

  <?php 
  if(!empty($open_journal_logo1)):?>
    <div id="open-journal-small-logo">
      <?php print l($open_journal_logo1, '', array('html' => TRUE));?>
    </div>
  <?php endif;?>


  <?php if ($title && !$disabled_title): ?>
    <h1 class="page-title"><?php print $title; ?></h1>
  <?php endif; ?>


  <div id="page">
    <div id="content" class="clearfix">
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
      <?php print render($page['content']); ?>
    </div>

  </div>
