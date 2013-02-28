jQuery(document).ready(function () {

  $ = jQuery;
  $('.multi-textfield').each(function () {
    if (!$(this).hasClass('multi-textfield-0') && !$(this).hasClass('multi-textfield-1') && $(this).val() == '') {
      $(this).parent('.form-type-textfield').hide();
    }
  });

  $('.add-multi-textfield').click(function (e) {
    e.preventDefault();
    $(this).siblings('.form-type-textfield:hidden:first').show();
    if (!$(this).siblings('.form-type-textfield:hidden').length) {
      $(this).hide();
      $(this).parent().append('<div class="description">Maximum to added are ' + $(this).siblings('.form-type-textfield').length + ' items.</div>');
    }
  });


});