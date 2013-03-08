jQuery(document).ready(function () {
  $ = jQuery;
  
  $('.user-dropdown .active-dropdown').hide();
  
  $('.user-dropdown a.dropdown-link').click(function (e) {
    
    e.preventDefault();

    $('.user-dropdown .active-dropdown').toggle();
  })

	$('body').click(function(e){
		if(!$(e.target).hasClass('dropdown-link')) {

		  $('.user-dropdown .active-dropdown').hide();
		}
		if(!$(e.target).hasClass('journal-filter') && !$(e.target).hasClass('journal-filter-active')) {
	  
	  	$('#journal-filter-item-list').hide();
	  }
	});


/*
	$('.journal-filter-item').click(function (e) {
		e.preventDefault();
		// Active menu checked
		$(this).siblings().removeClass('select-item');
		$(this).addClass('select-item');

		// Change status active name
		var statusName = $(this).children('a').html();
		$('.journal-filter-active').html(statusName);


		// Show active status only
		var journalListStatusGroup = '.journal-item-status'+$(this).attr('rel');
			
		if($(this).attr('rel') == 'All') {
			$('.journal-item').show();	
		}
		else {
			$(journalListStatusGroup).show();
			$('.journal-item ').not($(journalListStatusGroup)).hide();
		}
	});
	*/

});