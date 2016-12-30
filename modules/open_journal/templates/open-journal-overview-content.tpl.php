<!-- Abstract -->
<div class="journal-block abstract-block block-bg-shadow">
  
  <h2 class="journal-block-title">Alternative title</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<p><?php print $alt_title; ?></p>
  </div><!-- End Journal-block-content -->
  

  <h2 class="journal-block-title">Abstract</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<p><?php print $abstract; ?></p>
  </div><!-- End Journal-block-content -->
  
  <!-- Journal-file-download -->
  <div class="journal-file-download">
  
  	<div class="file-download-update">
  	  <span class="label">Latest file uploaded: </span>
  	  <span> <?php print $last_update;?></span>
  	</div>
  	
  	<div class="file-download-create">
  	  <span class="label">By: </span>
  	  <span><?php print $update_by; ?></span>		
  	</div>
  	<?php print l(t('Download'), $latest_file, array('attributes' => array('class' => 'file-download-btn')));?>
  </div><!-- End Journal-file-download -->
  
</div><!-- End Abstract Block -->	

<!-- Discussion -->	
<div class="journal-block discussion-block block-bg-shadow">

  <h2 class="journal-block-title">Discussion</h2>
  <div class="add-discussion-btn">
    <?php print l('Add discussion', OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion/add');?>
  </div>
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul id="overview-discussion">
      <?php foreach($discussion_data as $data):?>
      <li>
        <?php if ($data['contributor_only']): ?>
        <span class="contributor-only-icon">contributor only</span>  
        <?php endif ?>
        <div class="journal-user profile-picture-small">
          <?php print $data['user_info']['display_picture']; ?>
          <?php print $data['user_info']['display_name']; ?>
        </div>

        <div class="discus-title">
          <?php if ($data['auto']):?>
            <?php print $data['title']; ?>
          <?php elseif($data['fid'] || count($data['files_upload'])):?>
            <?php  print l($data['title'].'<span class="file-attach">'.$image_file.'</span>', OPEN_JOURNAL_PREFIX_PATH.'/'.$data['jid'].'/discussion/'.$data['did'], array('html' => TRUE));?>
          <?php else:?>
            <?php  print l($data['title'], OPEN_JOURNAL_PREFIX_PATH.'/'.$data['jid'].'/discussion/'.$data['did'], array('html' => TRUE));?>
          <?php endif;?>
        </div>

        <div class="discus-content"><?php print $data['description'];?></div>
        <?php //print open_journal_trim($title.$description);?>

      </li>
      <?php endforeach;?>
  	</ul>
  </div><!-- End Journal-block-content -->
  
  <?php if($more_discussion > 0):?>
  <?php print l($more_discussion.' more discussion', OPEN_JOURNAL_PREFIX_PATH.'/'.$jid.'/discussion', array('attributes' => array('class' => 'view-all-discus')));?>
  <?php endif;?>
</div><!-- End Discussion Block -->	
