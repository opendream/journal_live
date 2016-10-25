<!-- File List -->	
<div class="journal-block files-block block-bg-shadow">

  <h2 class="journal-block-title">Files</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul id="file-list-item">
  	  
  	  
  	  <?php foreach ($file_list as $file): ?>
  	  <li>
  			<div class="journal-user profile-picture-small">
  			  <?php print $file->user_info['display_picture']; ?>
  			  <?php print $file->user_info['display_name']; ?>
  			</div>
  			<div class="file-name">
  			  <span class="file-attach">
  			    <?php print $file_icon; ?>
  			  </span>
  			  <?php print l($file->filename, file_create_url($file->uri)); ?>
  			</div>
  			<div class="upload-date">
  				<span class="label">Date: </span>
  				<span><?php print date('d M Y', $file->timestamp); ?></span>
  			</div>
  			<div class="file-size">
  				<span class="label">Size: </span>
  				<span><?php print format_size($file->filesize); ?></span>	
  			</div>
  			<?php print l(t('Download'), file_create_url($file->uri), array('attributes' => array('class' => array('file-download-btn')))); ?>
  		</li>
  	  <?php endforeach ?>
  	</ul>
  </div><!-- End Journal-block-content -->	
  
</div><!-- End File List -->	