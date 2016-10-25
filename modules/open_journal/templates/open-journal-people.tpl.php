<!-- People -->
<div class="journal-block people-block">
  
  <div class="block-bg-shadow people-block-inner">
    
    <h2 class="journal-block-title">People</h2>

    <!-- Creator Section -->
    <div class="journal-block-content people-creator">

    	<h3 class="people-section-title">Creator</h3>

      <!-- People item -->
      <div class="journal-user">
        <?php print $people_creator->user_info['display_picture']; ?>
        <h3 class="username"><?php print $people_creator->user_info['display_name']; ?></h3>
        <div class="user-role">
            <span><?php print $people_creator->user_roles;?></span>
          </div>

      	<div class="join-date">
    			<span class="label">Join Date: </span>
    			<span><?php print date('d M Y', $people_creator->created); ?></span>		
    		</div>
      </div><!-- End People item -->

    </div><!-- End Creator Section -->

    <!-- Creator Section -->
    <div class="journal-block-content people-contributor">

    	<h3 class="people-section-title">Contributors</h3>

    	<!-- People item -->
    	<ul>
    	  <?php foreach ($people_contributor_list as $contributor): ?>
    	  <li class="journal-user<?php print $contributor->add_class; ?>">

    	    <?php print $contributor->user_info['display_picture']; ?>
    	    <h3 class="username"><?php print $contributor->user_info['display_name']; ?></h3>

          <div class="user-role">
            <span><?php print $contributor->user_roles;?></span>
          </div>

    	    <div class="join-date">
      			<span class="label">Join Date: </span>
      			<?php if ($contributor->active_in_journal && $contributor->created): ?>
      			<span><?php print date('d M Y', $contributor->created); ?></span>
      			<?php else: ?>
      			<span>&nbsp;&nbsp;-&nbsp;&nbsp;</span>
      			<?php endif ?>
      		</div>

          <?php print $contributor->form; ?>

    	  </li>
    	  <?php endforeach ?>
    	</ul><!-- End People item -->

    </div><!-- End Creator Section -->
    
  </div>
  
</div><!-- End People -->