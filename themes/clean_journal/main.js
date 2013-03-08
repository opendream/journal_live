jQuery(document).ready(function () {
  $ = jQuery;
  
  $('.user-dropdown .active-dropdown').hide();
  
  $('.user-dropdown a.dropdown-link').click(function (e) {
    
    e.preventDefault();
    
    $('.user-dropdown .active-dropdown').toggle();
  })

$('body').click(function(e){
	console.log(e.target);
	if(!$(e.target).hasClass('dropdown-link') && !$(e.target).hasClass('journal-filter-active')) {

	  $('.user-dropdown .active-dropdown').hide();
}
});


});