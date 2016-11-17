
<div class="report-filter block-bg-shadow">
	<h2 class="report-title">เลือกช่วงเวลา</h2>
	<div class="filter-date-wrapper filter-date-all">
		<div class="filter-date">
			<input type="radio" name="options" value="all" checked="checked"/>
		</div>
		<div class="filter-date filter-date-value selected">
			<span>ช่วงเวลาทั้งหมด (ปัจจุบัน)</span>
		</div>
	</div>
	<div class="filter-date-wrapper filter-date-range">
		<div class="filter-date">
			<input type="radio" name="options" value="range"/>
		</div>
		<div class="filter-date filter-date-value">
			<div class="filter-date filter-start-date">
				<div class="filter-start-date-label">
					<label>วันเริ่มต้น (ปี/เดือน/วัน)</label>
				</div>
				<div class="filter-start-date-input">
					<select name="start_year">
						<?php foreach ($year_list as $order => $value): ?>
						<option value="<?php print $value; ?>"<?php if ($order == (count($year_list) - 1)): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
					/
					<select name="start_month">
						<?php foreach ($month_list as $order => $value): ?>
						<option value="<?php print $value; ?>"<?php if ($order == 0): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
					/
					<select name="start_day">
						<?php foreach ($day_list as $order => $value): ?>
						<option value="<?php print $value; ?>"<?php if ($order == 0): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
				</div>
			</div>
			<div class="filter-date">
			→
			</div>
			<div class="filter-date filter-end-date">
				<div class="filter-date filter-end-date-label">
					<label>วันสิ้นสุด (ปี/เดือน/วัน)</label>
				</div>
				<div class="filter-end-date-input">
					<select name="end_year">
						<?php foreach ($year_list as $value): ?>
						<option value="<?php print $value; ?>"<?php if (date('Y') == $value): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
					/
					<select name="end_month">
						<?php foreach ($month_list as $value): ?>
						<option value="<?php print $value; ?>"<?php if (date('m') == $value): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
					/
					<select name="end_day">
						<?php foreach ($day_list as $value): ?>
						<option value="<?php print $value; ?>"<?php if (date('d') == $value): ?> selected="selected"<?php endif ?>><?php print $value; ?></option>
						<?php endforeach ?>
					</select>
				</div>
			</div>
		</div>
	</div>
</div>

<div class="report-list-wrapper">
	<?php foreach ($report_list as $report): ?>
	<div class="report-item block-bg-shadow">
		<div class="report-info">
			<h2 class="report-title"><?php print $report['title'] ?></h2>
			<div class="report-subtitle"><?php print $report['subtitle'] ?></div>
		</div>
		<div class="report-action">
			<a class="report-export" href="<?php print $report['url'] ?>">EXPORT</a>
		</div>
	</div>
	<?php endforeach ?>
	<?php print $content; ?>
</div>