jQuery(document).ready(function () {

  $ = jQuery;
  

  $('.add-multi-textfield').click(function (e) {
    e.preventDefault();
    $(this).siblings('.form-type-textfield:hidden:first').show();
    if (!$(this).siblings('.form-type-textfield:hidden').length) {
      $(this).hide();
      $(this).parent().append('<div class="description">Maximum to added are ' + $(this).siblings('.form-type-textfield').length + ' items.</div>');
    }
  });

  $('.multi-textfield-0').parent('.form-type-textfield').each(function () {

    $.each($.makeArray($(this).siblings('.form-type-textfield')).reverse(), function (i, item) {
      
      var input = $(item).find('input');
      if (input.val() || input.hasClass('multi-textfield-1')) {
        return false;
      }
      else {
        $(item).hide();
      }
    })
  });

  
});