<!-- Abstract -->
<div class="journal-block abstract-block block-bg-shadow">

  <h2 class="journal-block-title">Abstract</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<p><?php print $abstract; ?></p>
  </div><!-- End Journal-block-content -->
  
  <!-- Journal-file-download -->
  <div class="journal-file-download">
  
  	<div class="file-download-update">
  	  <span class="label">Last update: </span>
  	  <span><?php print $last_update;?></span>
  	</div>
  	
  	<div class="file-download-create">
  	  <span class="label">By: </span>
  	  <span><?php print $update_by; ?></span>		
  	</div>
  	
  	<a class="file-download-btn" href="<?php print $latest_file;?>" target="_blank">Download</a>
  </div><!-- End Journal-file-download -->
  
</div><!-- End Abstract Block -->	

<!-- Discussion -->	
<div class="journal-block discussion-block block-bg-shadow">

  <h2 class="journal-block-title">Discussion</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul id="overview-discussion">
      <?php foreach($discussion_data as $data):?>
      <li>
        <div class="journal-user profile-picture-small"><a href="#"><?php print $data['user_info']['display_picture'];?><span class="username"><?php print $data['user_info']['display_name'];?></span></a></div>
        <div class="discus-title"><a href="#"><?php print $data['title'];?><span class="file-attach"><img src="/sites/all/modules/open_journal/images/ic-has-file.png" alt="has file attach" /></span></a></div>
        <div class="discus-content"><?php print $data['description'];?></div>
      </li>
      <?php endforeach;?>
  	</ul>
  </div><!-- End Journal-block-content -->
  
  <?php if($more_discussion > 0):?>
  <a class="view-all-discus" href=""><?php print $more_discussion;?> more discussion</a>	
  <?php else:?>
  <span> no more discustion</span>
  <?php endif;?>
</div><!-- End Discussion Block -->	