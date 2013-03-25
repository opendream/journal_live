<!-- Start Status Block -->	
<div class="journal-block status-block block-bg-shadow">
	
	<div class="h2-wrapper">
		<h2 class="journal-block-title status<?php print $status['class'];?>"><?php print $status['name']?></h2>
	</div>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<div class="journal-update">
  		<span class="label">Status update: </span>
  		<span><?php print $status['updated']?></span>
  	</div>
  	<div class="journal-create">
  		<span class="label">Creation Date: </span>
  		<span><?php print $status['created']?></span>		
  	</div>
  </div><!-- End Journal-block-content -->

</div> <!-- End Status Block -->	

<!-- Start People Block -->	

<div class="journal-block people-block-overview block-bg-shadow">

  <h2 class="journal-block-title">People</h2>
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  	<ul class="relate-peoples">
      <?php foreach($people_list as $key => $list):?>
        <?php // this condition not clear ?>
        <?php if($key == 'more_people' && $key != '0'):?>
        <li class="more-people">
          <?php print l($list['user_info']['display_picture'], 'journal/'.$jid.'/people', array('attributes' => array('title' => $list['user_info']['display_name'])));?>
        </li>
        <?php else:?>
        <li class="clickable-people owner profile-picture-small">
          <?php print $list['user_info']['display_picture']; ?>  
        </li>  
        <?php endif;?>
      <?php endforeach;?>	
  	</ul>	
  </div><!-- End Journal-block-content -->
  <?php print l('View all people' , 'journal/'.$jid.'/people', array('attributes' => array('class' => 'view-all-people')));?>
</div> <!-- End People Block -->	

<!-- Start Information Block -->
<div class="journal-block information-block block-bg-shadow">

  <h2 class="journal-block-title">Information</h2>
  
  <!-- Journal-block-content -->
  <div class="journal-block-content">
  
  <?php foreach ($information as $value):?>
    <?php if($value['value'] != ''):?>
    <div class="information-field subject-en-field">
      <span class="label"><?php print $value['label'];?>:</span>
      <span class="value"><?php print $value['value']?></span>
    </div>
   <?php endif;?>
  <?php endforeach;?>
    
  </div><!-- End Journal-block-content -->
  
</div> <!-- End Information Block -->	
