jQuery(document).ready(function () {

  $ = jQuery;
  
  $('.toggle-people-btn').mouseenter(function () {
    if ($(this).hasClass('active-people-btn')) {
      $(this).val('deactive');
    }
    else {
      $(this).val('active');
    }
  }).mouseout(function () {
    if ($(this).hasClass('active-people-btn')) {
      $(this).val('active');
    }
    else {
      $(this).val('deactive');
    }
  });

  
});