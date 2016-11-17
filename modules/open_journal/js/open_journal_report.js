$ = jQuery;

$(document).ready(function () {

	var option = 'all';

	var get_date_filter = function () {

		var start_year = $('[name=start_year]').val();
		var start_month = $('[name=start_month]').val();
		var start_day = $('[name=start_day]').val();
		var end_year = $('[name=end_year]').val();
		var end_month = $('[name=end_month]').val();
		var end_day = $('[name=end_day]').val();

		var start_date = start_year + '-' + start_month + '-' + start_day;
		var end_date = end_year + '-' + end_month + '-' + end_day;

		return {start_date: start_date, end_date: end_date};

	}

	var update_query = function () {

		console.log(option);

		var query = '';
		if (option == 'range') {
			query = '?' + $.param(get_date_filter());
		}

		$('a.report-export').each(function () {
			var href = $(this).attr('href').split('?')[0];
			$(this).attr('href', href + query);

		})

	}

	$('input[name=options]').change(function() {

		option = $(this).val();

		$('.filter-date-value').removeClass('selected');
		$('.filter-date-' + option + ' .filter-date-value').addClass('selected');

		update_query();

	});

	$('.filter-start-date-input select, .filter-end-date-input select').change(function() {
		update_query();
	});

	$('input[name=options]').change();

});