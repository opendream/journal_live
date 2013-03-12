
<?php if ($jid): ?>
<div class="journal-menu">
  <ul id="journal-menu-item-list">
    <li><?php print l(t('Overview'),    OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/overview'); ?></li>
    <li><?php print l(t('Discussions'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion', open_journal_get_active_menu_attributes($menu_active_url)); ?></li>
    <li><?php print l(t('Information'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/information'); ?></li>
    <li><?php print l(t('Files'),       OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/file'); ?></li>
    <!--<li><?php print l(t('Peoples'),     OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/people'); ?></li>-->
  </ul>
</div>
<?php endif ?>

<div id="journal-detail-wrapper" class="journal-detail-normal<?php if ($sidebar): ?> journal-detail-has-sidebar<?php endif; ?> ">
  <div id="journal-detail-content">

    <?php print $content;?>

  </div>
  <div id="journal-detail-sidebar">
    <?php print $sidebar; ?>    
  </div>
</div>

