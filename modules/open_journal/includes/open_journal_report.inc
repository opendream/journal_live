<?php


function open_journal_report_list() {
  $mpath = drupal_get_path('module', 'open_journal');
  drupal_add_js($mpath.'/js/open_journal_report.js');

  $report_list = array(
  	//5.1
    array(
      'title' => 'รายงานบทความ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, สถานะปัจจุบัน, วันที่ส่งบทความ และ วันสุดท้ายที่มีการเปลี่ยนแปลงสถานะครั้งล่าสุด',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-list')
    ),
    //5.2, 5.6, 5.7
    array(
      'title' => 'รายงานจำนวนบทความ',
      'subtitle' => 'แสดง : จำนวนบทความในแต่ละสถานะทั้งแบบรวมและไม่รวมบทความที่ตีพิมพ์แล้ว และแจกแจงเมื่อมีการตีพิมพ์คนละปี',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-summary')
    ),
    //5.3
    array(
      'title' => 'รายงานบทความที่ตีพิมพ์',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-publication')
    ),
    //5.4
    array(
      'title' => 'รายงานบทความที่ปฏิเสธ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ปฏิเสธ, จำนวนวันจากวันที่ส่งบทความถึงวันที่ปฏิเสธ, สถานะ ณ วันที่ปฏิเสธ และ เหตุผล',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-reject')
    ),
    //5.5
    array(
      'title' => 'รายงานบทความที่ถอน',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ถอน, จำนวนวันจากวันที่ส่งบทความถึงวันที่ถอน, สถานะ ณ วันที่ถอน และ เหตุผล',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-withdraw')
    ),
    //5.8
    array(
      'title' => 'รายงานบทความ และผู้ทรงคุณวุฒิ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก และ รายชื่อผู้ทรงคุณวุฒิ',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/journal-reviewer')
    ),
    //5.9
    array(
      'title' => 'รายงานผลการทำงานของผู้ทรงคุณวุฒิ',
      'subtitle' => 'แสดง : ชื่อผู้ทรงคุณวุฒิ จำนวนบทความที่ได้พิจารณา และรายชื่อบทความ',
      'url' => url(OPEN_JOURNAL_PREFIX_PATH.'/report/reviewer-summary')
    ),
  );

  $items['attributes'] = array(
  	'class' => 'block-bg-shadow'
  );

  $oldest = db_query("SELECT created FROM {open_journal_journal} ORDER BY created LIMIT 0, 1")->fetchField();

  return theme('open_journal_report_list', array(
  	'year_list' => range(date('Y'), date('Y', $oldest), -1),
  	'month_list' => range(1, 12, 1),
  	'day_list' => range(1, 31, 1),
    'report_list' => $report_list
  ));;
}


// 5.1
function open_journal_report_journal_list() {
  return 'open_journal_report_journal_list';
}

// 5.2 5.6 5.7
function open_journal_report_journal_summary() {
  return 'open_journal_report_journal_summary';
}

// 5.3
function open_journal_report_journal_publication() {
  return 'open_journal_report_journal_publication';
}

// 5.4
function open_journal_report_journal_reject() {
  return 'open_journal_report_journal_reject';
}

// 5.5
function open_journal_report_journal_withdraw() {
  return 'open_journal_report_journal_withdraw';
}

// 5.8
function open_journal_report_journal_reviewer() {
  return 'open_journal_report_journal_reviewer';
}

// 5.9
function open_journal_report_reviewer_journal() {
  return 'open_journal_report_reviewer_journal';
}
