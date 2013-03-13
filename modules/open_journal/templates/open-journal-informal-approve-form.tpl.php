<div class="informal-status-block block-bg-shadow">
  
  <h2 class="journal-block-title">Informal status</h2>
  
  <div class="journal-informal-approve-wrapper">
    <ul class="journal-informal-approve-list">
      <?php foreach ($informal_list as $informal): ?>
      <li class="journal-informal-approve-item<?php print $informal->add_class; ?>">
        <?php print $informal->user_info['display_picture']; ?>
        <?php print $informal->user_info['display_name']; ?>
        
        <div class="informal-action">
          <?php print $informal->form; ?>
        </div>
        
      </li>
      <?php endforeach ?>
    </ul>
  </div>

</div>