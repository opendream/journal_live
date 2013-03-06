
jQuery(document).ready(function(){

  var $ = jQuery;

  var compact_blur = function (self) {
    if (!$(self).val()) {
      $(self).siblings('label').show();
    }
    else {
      $(self).siblings('label').hide();
    }
  }

  $('.form-type-textfield input, .form-type-password input')
  .focus(function () {
    $(this).siblings('label').hide();
  })
  .blur(function () {
    compact_blur(this);
  });

  $('.form-type-textfield label, .form-type-password label').hide();
  setTimeout(function () {
    $('.form-type-textfield input, .form-type-password input').each(function (i, item) {
      compact_blur(item);
    });
  }, 500);

});
