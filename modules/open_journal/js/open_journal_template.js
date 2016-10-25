$ = jQuery;
// For faster hide form
$(document).ready(function () {
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
		if ($(this).val() != 'none') {

			$('.form-type-plupload').show();
			// Clear select role
			$('#edit-roles').find('input').attr({'checked': false});
			var select_value = $(this).val();
			var text = template_list[select_value]['body'];
			$('#wmd-input-editor').val(text);

			$.each(template_list[select_value]['role'], function( index, value ) {
				$('#edit-roles-'+value['rid']).attr({'checked': true});
			})

		}
		else {
			$('#wmd-input-editor').val('');
			$('.form-type-plupload').hide();
			$('#select_all').trigger('click');

		}

	});

	$('#select_all').css({'cursor': 'pointer'});
	$('#select_none').css({'cursor': 'pointer'});
	$('#select_all').click(function () {

		$("#edit-roles > .form-item").each(function( index ) {
			$(this).find('input').attr({'checked': true});
		});

	});
	$('#select_none').click(function () {

		$("#edit-roles > .form-item").each(function( index ) {
			$(this).find('input').attr({'checked': false});
		});

	});

});