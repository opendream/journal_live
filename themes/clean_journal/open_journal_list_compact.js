
jQuery(document).ready(function(){

  var $ = jQuery;
  
  $('.journal-filter').siblings('ul').hide();

  var compact_blur = function (self) {
    
      $(self).siblings('ul').toggle();
    
  }

  $('.journal-filter')
  .click(function () {
    compact_blur(this);
  });

  $('body').click(function(e){
    if(!$(e.target).hasClass('journal-filter') && !$(e.target).hasClass('journal-filter-active')) {
    
    $('#journal-filter-item-list').hide();
    }
  });
  
});
