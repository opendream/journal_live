
<?php if ($jid): ?>
<ul id="journal-detail-navigation">
  <li><?php print l(t('Overview'),    OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.''); ?></li>
  <li><?php print l(t('Discussions'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.''); ?></li>
  <li><?php print l(t('Information'), OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/information'); ?></li>
  <li><?php print l(t('Files'),       OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.''); ?></li>
  <li><?php print l(t('Peoples'),     OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.''); ?></li>
</ul>
<?php endif ?>

<div id="journal-detail-content">
  <?php print $content; ?>    
</div>

