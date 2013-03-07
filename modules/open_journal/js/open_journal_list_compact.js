
jQuery(document).ready(function(){

  var $ = jQuery;
  
  $('.journal-filter').siblings('ul').hide();

  var compact_blur = function (self) {
    
      $(self).siblings('ul').toggle();
    
  }

  $('.journal-filter ')
  .click(function () {
    //$(this).siblings('ul').hide();
    compact_blur(this);
  })
  .blur(function () {
    $(this).siblings('ul').hide();
  });

  
});
