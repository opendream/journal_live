<?php


function open_journal_report_data() {
  return array(
    //5.1
    array(
      'title' => 'รายงานบทความ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, สถานะปัจจุบัน, วันที่ส่งบทความ และ วันสุดท้ายที่มีการเปลี่ยนแปลงสถานะครั้งล่าสุด',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-list',
      'callback' => 'open_journal_report_journal_list'
    ),
    //5.2, 5.6, 5.7
    array(
      'title' => 'รายงานจำนวนบทความ',
      'subtitle' => 'แสดง : จำนวนบทความในแต่ละสถานะทั้งแบบรวมและไม่รวมบทความที่ตีพิมพ์แล้ว และแจกแจงเมื่อมีการตีพิมพ์คนละปี',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-summary',
      'callback' => 'open_journal_report_journal_summary'
    ),
    //5.3
    array(
      'title' => 'รายงานบทความที่ตีพิมพ์',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-publication',
      'callback' => 'open_journal_report_journal_publication'
    ),
    //5.4
    array(
      'title' => 'รายงานบทความที่ปฏิเสธ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ปฏิเสธ, จำนวนวันจากวันที่ส่งบทความถึงวันที่ปฏิเสธ, สถานะ ณ วันที่ปฏิเสธ และ เหตุผล',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-reject',
      'callback' => 'open_journal_report_journal_reject'
    ),
    //5.5
    array(
      'title' => 'รายงานบทความที่ถอน',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ถอน, จำนวนวันจากวันที่ส่งบทความถึงวันที่ถอน, สถานะ ณ วันที่ถอน และ เหตุผล',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-withdraw',
      'callback' => 'open_journal_report_withdraw'
    ),
    //5.8
    array(
      'title' => 'รายงานบทความ และผู้ทรงคุณวุฒิ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก และ รายชื่อผู้ทรงคุณวุฒิ',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-reviewer',
      'callback' => 'open_journal_report_journal_reviewer'
    ),
    //5.9
    array(
      'title' => 'รายงานผลการทำงานของผู้ทรงคุณวุฒิ',
      'subtitle' => 'แสดง : ชื่อผู้ทรงคุณวุฒิ จำนวนบทความที่ได้พิจารณา และรายชื่อบทความ',
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/reviewer-summary',
      'callback' => 'open_journal_report_reviewer_journal'
    ),
  );
}

function open_journal_get_oldest() {
  return db_query("SELECT created FROM {open_journal_journal} ORDER BY created LIMIT 0, 1")->fetchField();
}

function open_journal_get_query() {
  
  if (!$_GET['start_date']) {
    $start_date = open_journal_get_oldest();
  }
  else {
    $start_date = strtotime($_GET['start_date']);
  }

  if (!$_GET['end_date']) {
    $end_date = REQUEST_TIME;
  }
  else {
    $end_date = strtotime($_GET['end_date']);
  }

  return array('start_date' => $start_date, 'end_date' => $end_date);
}

function open_journal_get_filename($prefix) {
    if ($_GET['start_date']) {
      $prefix .= '--'.$_GET['start_date'];
    }
    if ($_GET['end_date']) {
      $prefix .= '--'.$_GET['end_date'];
    }
    return $prefix;
}

function open_journal_result_to_csv($result) {

  drupal_add_http_header('Content-Type', 'text/csv');
  drupal_add_http_header('Content-Disposition', 'attachment;filename='.open_journal_get_filename('report-journal-list').'.csv');

  $fp = fopen('php://output', 'w');
  $first = TRUE;

  foreach ($result as $line) {
     //Gets the line so we can flip it and get the column names
    $column_names = get_object_vars($line); 
    if ($first) {
      $field_names = array_flip($column_names);
      fputcsv($fp, $field_names);
      $first = FALSE; // Only happens once
    }
    // Puts the actual content
    fputcsv($fp, $column_names);
  }
  fclose($fp);
  drupal_exit();
}

function open_journal_report_list() {
  $mpath = drupal_get_path('module', 'open_journal');
  drupal_add_js($mpath.'/js/open_journal_report.js');

  $report_list = open_journal_report_data();

  $items['attributes'] = array(
  	'class' => 'block-bg-shadow'
  );

  $oldest = open_journal_get_oldest();

  return theme('open_journal_report_list', array(
  	'year_list' => range(date('Y'), date('Y', $oldest), -1),
  	'month_list' => range(1, 12, 1),
  	'day_list' => range(1, 31, 1),
    'report_list' => $report_list
  ));;
}



// 5.1
function open_journal_report_journal_list() {
  /*
  'title' => 'รายงานบทความ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, สถานะปัจจุบัน, วันที่ส่งบทความ และ วันสุดท้ายที่มีการเปลี่ยนแปลงสถานะครั้งล่าสุด',
  */
  $query = open_journal_get_query();
  $result = db_query("SELECT 
      j.jid `ID`, 
      j.title_en `Title`, 
      j.title_sub `Sub title`, 
      s.name `State name`, 
      DATE_FORMAT(FROM_UNIXTIME(j.created), '%Y-%m-%d %T') `Created at`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%m-%d %T') `Last state changed at` 
    FROM 
      {open_journal_journal} j, 
      {open_journal_state} s 
    WHERE 
      j.sid = s.sid AND j.reject = 0
  ");
  // TODO: j.jid -> j.code, WHERE j.withdraw = 0

  open_journal_result_to_csv($result);

}

// 5.2 5.6 5.7
function open_journal_report_journal_summary() {
  /*
  'title' => 'รายงานจำนวนบทความ',
  'subtitle' => 'แสดง : จำนวนบทความในแต่ละสถานะทั้งแบบรวมและไม่รวมบทความที่ตีพิมพ์แล้ว และแจกแจงเมื่อมีการตีพิมพ์คนละปี',
  */
  return 'open_journal_report_journal_summary';
}

// 5.3
function open_journal_report_journal_publication() {
  /*
  'title' => 'รายงานบทความที่ตีพิมพ์',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
  */
  $query = open_journal_get_query();
  return 'open_journal_report_journal_publication';
}

// 5.4
function open_journal_report_journal_reject() {
  /*
  'title' => 'รายงานบทความที่ปฏิเสธ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ปฏิเสธ, จำนวนวันจากวันที่ส่งบทความถึงวันที่ปฏิเสธ, สถานะ ณ วันที่ปฏิเสธ และ เหตุผล',
  */
  $query = open_journal_get_query();
  return 'open_journal_report_journal_reject';
}

// 5.5
function open_journal_report_journal_withdraw() {
  /*
  'title' => 'รายงานบทความที่ถอน',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ถอน, จำนวนวันจากวันที่ส่งบทความถึงวันที่ถอน, สถานะ ณ วันที่ถอน และ เหตุผล',
  */
  $query = open_journal_get_query();
  return 'open_journal_report_journal_withdraw';
}

// 5.8
function open_journal_report_journal_reviewer() {
  /*
  'title' => 'รายงานบทความ และผู้ทรงคุณวุฒิ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก และ รายชื่อผู้ทรงคุณวุฒิ',
  */
  $query = open_journal_get_query();
  return 'open_journal_report_journal_reviewer';
}

// 5.9
function open_journal_report_reviewer_journal() {
  /*
  'title' => 'รายงานผลการทำงานของผู้ทรงคุณวุฒิ',
  'subtitle' => 'แสดง : ชื่อผู้ทรงคุณวุฒิ จำนวนบทความที่ได้พิจารณา และรายชื่อบทความ',
  */
  $query = open_journal_get_query();
  return 'open_journal_report_reviewer_journal';
}
