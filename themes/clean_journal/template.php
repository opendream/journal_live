<?php

function clean_journal_preprocess_page(&$vars) {
  
  $status = drupal_get_http_header("status");
  if($status == "404 Not Found") {
    $vars['page']['content'] = theme('image', array('path' => drupal_get_path('theme', 'clean_journal').'/images/404.png'));
  }  
}