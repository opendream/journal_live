<!-- People -->
<div class="journal-block people-block block-bg-shadow">

  <h2 class="journal-block-title">People</h2>
  
  <!-- Creator Section -->
  <div class="journal-block-content people-creator">
  
  	<h3 class="people-section-title">Creator</h3>
    
    <!-- People item -->
    <div class="journal-user">
      <?php print $people_creator->user_info['display_picture']; ?>
      <h3 class="username"><?php print $people_creator->user_info['display_name']; ?></h3>
    	<div class="join-date">
  			<span class="label">Join Date: </span>
  			<span><?php print date('d M Y', $people_creator->joined); ?></span>		
  		</div>
    </div><!-- End People item -->
    
  </div><!-- End Creator Section -->

  <!-- Creator Section -->
  <div class="journal-block-content people-contributor">
  
  	<h3 class="people-section-title">Contributor</h3>
  	
  	<!-- People item -->
  	<ul>
  	  <?php foreach ($people_contributor_list as $contributor): ?>
  	  <li class="journal-user<?php print $contributor->add_class; ?>">
  	    
  	    <?php print $contributor->user_info['display_picture']; ?>
  	    <h3 class="username"><?php print $contributor->user_info['display_name']; ?></h3>
  	    
  	    <div class="join-date">
    			<span class="label">Join Date: </span>
    			<?php if ($contributor->active_in_journal && $contributor->joined): ?>
    			<span><?php print date('d M Y', $contributor->joined); ?></span>
    			<?php else: ?>
    			<span>&nbsp;&nbsp;-&nbsp;&nbsp;</span>
    			<?php endif ?>
    		</div>
    		
    		<?php if ($contributor->active_in_journal): ?>
    		<a class="active-people-btn" href="/">active</a>
    		<?php else: ?>
    		<a class="inactive-people-btn" href="/">inactive</a>
    		<?php endif ?>

  	  </li>
  	  <?php endforeach ?>
  	</ul><!-- End People item -->
    
  </div><!-- End Creator Section -->
  
</div><!-- End People -->