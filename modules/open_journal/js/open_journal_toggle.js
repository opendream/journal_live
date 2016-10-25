jQuery(document).ready(function () {

  $ = jQuery;
  
  $('.toggle-people-btn').mouseenter(function () {
    if ($(this).hasClass('active-people-btn')) {
      $(this).val('deactivate');
    }
    else {
      $(this).val('activate');
    }
  }).mouseout(function () {
    if ($(this).hasClass('active-people-btn')) {
      $(this).val('activate');
    }
    else {
      $(this).val('deactivate');
    }
  });

  
});