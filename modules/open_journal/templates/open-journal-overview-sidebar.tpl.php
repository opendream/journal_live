<!-- Start Status Block -->	
<div class="journal-block status-block block-bg-shadow">

  <h2 class="journal-block-title"><?php print $status['name']?></h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<div class="journal-update">
  		<span class="label">Status update: </span>
  		<span><?php print $status['updated']?></span>
  	</div>
  	<div class="journal-create">
  		<span class="label">Create Date: </span>
  		<span><?php print $status['created']?></span>		
  	</div>
  </div><!-- End Journal-block-content -->

</div> <!-- End Status Block -->	

<!-- Start People Block -->	
<div class="journal-block people-block block-bg-shadow">

  <h2 class="journal-block-title">People</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul class="relate-peoples">
      <?php foreach($people_list as $list):?>
        <?php if($list['user_info']['display_picture'] == 'more'):?>
        <li class="more-people">
          <?php print l('...', '', array('attributes' => array('title' => $list['user_info']['display_name'])));?>
        </li>
        <?php else:?>
        <li class="clickable-people owner profile-picture-small">
          <?php if($information->uid == $list['uid']):?>
            <?php print l($list['user_info']['display_picture'].'<span>Owner</span>', '' ,array('html' => TRUE))?>
          <?php else:?>
            <?php print l($list['user_info']['display_picture'], '', array('html' => TRUE))?>  
          <?php endif;?>
        </li>  
        <?php endif;?>
      <?php endforeach;?>	
  	</ul>	
  </div><!-- End Journal-block-content -->
  <?php print l('View all people' ,'', array('attributes' => array('class' => 'view-all-people')));?>
</div> <!-- End People Block -->	

<!-- Start Information Block -->
<div class="journal-block information-block block-bg-shadow">

  <h2 class="journal-block-title">Information</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  
    <?php if (OPEN_JOURNAL_SUB_LANG_ENABLE):?>
    <div class="information-field subject-sub-field">
    <span class="label"><?php print OPEN_JOURNAL_SUB_LANG_KEYWORDS_LABEL ; ?>: </span>
    <span class="value"><?php print $information->subject_sub;?> </span>
    </div>
    <?php endif;?>

    <?php if ($information->subject_en): ?>
  	<div class="information-field subject-en-field">
  		<span class="label">Keyword:</span>
  		<span class="value"><?php print $information->subject_en;?></span>
  	</div>
    <?php endif;?>

    <?php if ($information->dc_creator): ?>
  	<div class="information-field dc-creator-field">
  		<span class="label">Creators: </span>
  		<span class="value"><?php echo $information->dc_creator;?></span>
  	</div>
    <?php endif;?>
  	
    <?php if ($information->dc_publisher): ?>
  	<div class="information-field dc-publisher-field">
  		<span class="label">Publishers: </span>
  		<span class="value"><?php print $information->dc_publisher?></span>
  	</div>
    <?php endif;?>
  	
    <?php if ($information->dc_contributor): ?>
  	<div class="information-field dc-contributor-field">
  		<span class="label">Contributors: </span>
  		<span class="value"><?php print $information->dc_contributor;?></span>
  	</div>
    <?php endif;?>

    <?php if ($information->dc_date): ?>
  	<div class="information-field date-field">
  		<span class="label">Date: </span>
  		<span class="value"><?php print date('d M Y' ,$information->dc_date);?></span>
  	</div>
    <?php endif;?>

    <?php if ($information->dc_language): ?>
  	<div class="information-field language-field">
  		<span class="label">Language: </span>
  		<span class="value"><?php print $information->dc_language?></span>
  	</div>
    <?php endif;?>

    <?php if ($information->dc_identifier): ?>
  	<div class="information-field dc-identifier-field">
  		<span class="label">Final URL: </span>
  		<span class="value"><?php print $information->dc_identifier;?></span>
  	</div>
    <?php endif;?>
  	
  	

  </div><!-- End Journal-block-content -->
  
</div> <!-- End Information Block -->	
