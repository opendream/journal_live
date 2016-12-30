$ = jQuery;
// For faster hide form
$(document).ready(function () {

	// Hide current user roles from checklist
	var current_user_roles = Drupal.settings.open_journal.current_user_roles;
	$.each(current_user_roles, function( index, value ) {
		$('#edit-roles').find('.form-item-roles-'+index).hide();
	});

	$('.form-item-roles > label').after($('.select-roles-control'));
	$('.select-roles-control').show();
	$('.form-type-plupload').hide();
	$('.form-item-contributor-only > label').css({'text-decoration': 'line-through'});
	$('.form-item-contributor-only > .description').css({'text-decoration': 'line-through'});
	
	var template_list = Drupal.settings.open_journal.template_list;
	var previous_template;

	$('#edit-template').click(function () {
        // Store the current value on focus and on change
        previous_template = this.value;
    });

	$('#edit-template').change(function () {
		// Check information fill change
		var title_value = $('#edit-title').val();
		var description_value = $('#wmd-input-editor').val();
		var roles_checked_count = $('#edit-roles').find('input[type="checkbox"]:checked').length;
		var roles_count = $('#edit-roles').find('input[type="checkbox"]').length;

		if (title_value != '' || description_value != '' || roles_checked_count != roles_count) {
			var confirm_template_change = confirm("The information you fill changed");
			if (!confirm_template_change) {
				$(this).val(previous_template);
				return false;

			}

		}

		// Todo rit change hi-so dialog and change wording
		if (('.form-type-plupload').length) {

			$('.form-type-plupload').show();
			// Clear select role
			$('#select_none').trigger('click');
			var select_value = $(this).val();
			var text = template_list[select_value]['body'];
			var template_title = template_list[select_value]['template_title'];
			$('#wmd-input-editor').val(text);
			$('#edit-title').val(template_title);

			$.each(template_list[select_value]['role'], function( index, value ) {
				$('#edit-roles-'+value['rid']).attr({'checked': true});
			})

		}
		else {
			$('#wmd-input-editor').val('');
			$('#edit-title').val('');
			$('.form-type-plupload').hide();
			$('#select_all').trigger('click');

		}

	});
	$('#edit-template').change();

	$('#select_all').css({'cursor': 'pointer'});
	$('#select_none').css({'cursor': 'pointer'});
	$('#select_all').click(function () {

		$("#edit-roles > .form-item").each(function( index ) {
			if($(this).css('display') != 'none') {
				$(this).find('input').attr({'checked': true});

			}
		});

	});
	$('#select_none').click(function () {

		$("#edit-roles > .form-item").each(function( index ) {
			if($(this).css('display') != 'none') {
				$(this).find('input').attr({'checked': false});
			}
		});

	});

});
