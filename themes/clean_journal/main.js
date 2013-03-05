jQuery(document).ready(function () {
  $ = jQuery;
  
  $('.user-dropdown .active-dropdown').hide();
  
  $('.user-dropdown a.dropdown-link').click(function (e) {
    
    e.preventDefault();
    
    $('.user-dropdown .active-dropdown').toggle();
  })
});