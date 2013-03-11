
jQuery(document).ready(function(){

  var $ = jQuery;

  var compact_blur = function (self) {
    
    
    var value = $(self).val();
    
    if ($(self).is('textarea')) {
      self = $(self).parent().get(0);
    }    
    
    if (!value) {
      $(self).siblings('label').show();
    }
    else {
      $(self).siblings('label').hide();
    }
  }

  var compact_selector = $('.form-type-textfield input, .form-type-password input, .form-type-textarea textarea');
  
  compact_selector.focus(function () {
    if ($(this).is('textarea')) {
      $(this).parent().siblings('label').hide();
    }
    else {
      $(this).siblings('label').hide();
    }
  })
  .blur(function () {
    compact_blur(this);
  });

  var compact_label_selector = '.form-type-textfield label, .form-type-password label, .form-type-textarea label';
  $(compact_label_selector).hide();
  
  setTimeout(function () {
    compact_selector.each(function (i, item) {
      compact_blur(item);
    });
  }, 500);
  
});
