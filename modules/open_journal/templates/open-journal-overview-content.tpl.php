<!-- Abstract -->
<div class="journal-block abstract-block block-bg-shadow">

  <h2 class="journal-block-title"><?php print $abstract_label;?></h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<p><?php print $abstract; ?></p>
  </div><!-- End Journal-block-content -->
  
  <!-- Journal-file-download -->
  <div class="journal-file-download">
  
  	<div class="file-download-update">
  	  <span class="label">Lastes file upload: </span>
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
    <?php print l('Add discussion', OPEN_JOURNAL_PREFIX_PATH.'/add');?>
  </div>
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul id="overview-discussion">
      <?php foreach($discussion_data as $data):?>
      <li>
        <div class="journal-user profile-picture-small">
          <?php print $data['user_info']['display_picture'].'<span class="username">'.$data['user_info']['display_name'].'</span>';?>
        </div>
        <div class="discus-title">
          <?php  print l($data['title'].'<span class="file-attach"><img src="/sites/all/modules/open_journal/images/ic-has-file.png" alt="has file attach" /></span>', current_path(), array('html' => TRUE));?>
          
        </div>
        <div class="discus-content"><?php print $data['description'];?></div>
        <?php //print open_journal_trim($title.$description);?>

      </li>
      <?php endforeach;?>
  	</ul>
  </div><!-- End Journal-block-content -->
  
  <?php if($more_discussion > 0):?>
  <a class="view-all-discus" href=""><?php print $more_discussion;?> more discussion</a>	
  <?php endif;?>
</div><!-- End Discussion Block -->	