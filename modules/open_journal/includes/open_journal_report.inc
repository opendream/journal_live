<?php
function open_journal_reviewer_form() {
  $rs = db_query("SELECT 
      u.uid `uid`,
      u.name `name`
    FROM 
      {users} u 
    LEFT JOIN 
      (
        SELECT * 
        FROM 
          (
            SELECT jid, uid, status 
            FROM  {open_journal_journal_people} 
            WHERE role_list LIKE '%reviewer%' 
            ORDER BY created DESC
          ) jp 
        GROUP BY jp.jid, jp.uid 
        HAVING jp.status = 1
      ) jp 
    ON 
      u.uid = jp.uid 
    WHERE 
      u.uid > 0
    GROUP BY 
      jp.uid")->fetchAll();

  $reviewers = array();
  foreach ($rs as $line) {
    $reviewer = user_load($line->uid);
    $reviewers[$reviewer->uid] = $reviewer->data['firstname'].' '.$reviewer->data['lastname'].' [uid:'.$reviewer->uid.']';
  }

  return array(
    'uid' => array(
      '#title' => 'Select reviewer',
      '#type' => 'select',
      '#options' => $reviewers
    )
  );
}

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
      'callback' => 'open_journal_report_journal_withdraw'
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
      'callback' => 'open_journal_report_reviewer_journal',
      'extra_form' => 'open_journal_reviewer_form',
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
    $end_date = strtotime($_GET['end_date'] . ' + 1 days');
  }

  return array(':start_date' => $start_date, ':end_date' => $end_date);
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

function open_journal_result_to_xlsx($result, $filename) {

  include_once("xlsxwriter.class.php");

  $filename = str_replace(array('open_journal_', '_'), array('', '-'), $filename);

  $filename = open_journal_get_filename($filename).".".date("Y-m-d-His", REQUEST_TIME).".xlsx";
  header('Content-disposition: attachment; filename="'.XLSXWriter::sanitize_filename($filename).'"');
  header("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet; charset=utf-8");
  header('Content-Transfer-Encoding: binary');
  header('Cache-Control: must-revalidate');
  header('Pragma: public');

  $header = array();

  $data = array();
  $is_first = TRUE;
  foreach ($result as $line) {
    if ($is_first) {
      $is_first = FALSE;
      $header = get_object_vars($line);
    }
    $data[] = (array) $line;
  }

  foreach ($header as $k => $v) {
    $header[$k] = 'string';
  }

  $writer = new XLSXWriter();
  $writer->writeSheet($data, 'Sheet1', $header);
  $writer->writeToStdOut();

  drupal_exit();

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
      $field_names = array_keys($column_names);
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
  // TODO: j.jid -> j.code, WHERE j.withdraw = 0

  $query = open_journal_get_query();
  $result = db_query("SELECT 
      j.jid `ID`, 
      j.code `Code`, 
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`, 
      s.name `State`, 
      j.address `Offices`,
      DATE_FORMAT(FROM_UNIXTIME(j.created), '%Y-%m-%d %T') `Created at`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%m-%d %T') `Last state changed at` 
    FROM 
      {open_journal_journal} j, 
      {open_journal_state} s 
    WHERE 
      j.sid = s.sid AND j.reject = 0 AND j.withdraw = 0 AND j.created >= :start_date AND j.created <= :end_date
    ORDER BY
      j.created DESC
  ", $query);


  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.2 5.6 5.7
function open_journal_report_journal_summary() {
  /*
  'title' => 'รายงานจำนวนบทความ',
  'subtitle' => 'แสดง : จำนวนบทความในแต่ละสถานะทั้งแบบรวมและไม่รวมบทความที่ตีพิมพ์แล้ว และแจกแจงเมื่อมีการตีพิมพ์คนละปี',
  */
  $query = open_journal_get_query();

  /*
  SELECT s.name, COUNT(j.jid) `include published`, IFNULL(COUNT(j.jid) - SUM(j.sid = 3), 0) `exclude published`  FROM open_journal_state s LEFT JOIN (SELECT * FROM (SELECT sid, jid FROM open_journal_journal_state WHERE start_date <= 1479457121 ORDER BY start_date DESC, jsid DESC) sjs GROUP BY sjs.jid) js ON s.sid = js.sid LEFT JOIN open_journal_journal j ON js.jid = j.jid GROUP BY s.sid;
  */

  $status_list = array_keys(open_journal_get_status_list(TRUE));

  $query = array(
    ':last_sid' => open_journal_get_last_state(),
    ':start_date' => $query[':start_date'],
    ':end_date' => $query[':end_date']
  );

  $published_years = db_query("SELECT 
      DATE_FORMAT(FROM_UNIXTIME(start_date), '%Y') year 
    FROM open_journal_journal_state 
    WHERE start_date >= :start_date AND start_date <= :end_date AND sid = :last_sid  
    GROUP BY year
  ", $query)->fetchAll();

  $cols = "";
  if(count($published_years) > 0) {
    foreach ($published_years as $year) {
      $cols .= "IFNULL(SUM(j.sid = :last_sid AND DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y') = '".$year->year."'), 0) `Published in ".$year->year."`, ";
    }
  }

  $result = db_query("SELECT 
      s.name `State`, 
      $cols
      IFNULL(COUNT(j.jid) - SUM(j.sid = :last_sid), 0) `Unpublished`, 
      COUNT(j.jid) `Total`
    FROM 
      {open_journal_state} s 
    LEFT JOIN 
      (
        SELECT * FROM 
          (
            SELECT sid, jid 
            FROM {open_journal_journal_state} 
            WHERE start_date <= :end_date  
            ORDER BY start_date DESC, jsid DESC
          ) sjs 
        GROUP BY sjs.jid
      ) js 
    ON 
      s.sid = js.sid 
    LEFT JOIN 
      {open_journal_journal} j 
    ON 
      js.jid = j.jid 
    WHERE
      j.reject = 0 AND j.withdraw = 0 AND j.created >= :start_date
    GROUP BY 
      s.sid
    ORDER BY FIELD(s.sid, ".implode(', ', $status_list).")
  ", $query);

  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.3
function open_journal_report_journal_publication() {
  /*
  'title' => 'รายงานบทความที่ตีพิมพ์',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
  */
  // TODO: j.jid -> j.code, WHERE j.withdraw = 0
  $query = open_journal_get_query();
  $query[':last_sid'] = open_journal_get_last_state();

  $result = db_query("SELECT 
      j.jid `ID`, 
      j.code `Code`, 
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `Author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `Author (th)`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%m-%d %T') `Published at`, 
      j.address `Offices`
    FROM 
      {open_journal_journal} j  
    WHERE 
      j.sid = :last_sid AND j.reject = 0 AND j.status_changed >= :start_date AND j.status_changed <= :end_date 
    ORDER BY
      j.status_changed DESC
  ", $query);

  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.4
function open_journal_report_journal_reject() {
  /*
  'title' => 'รายงานบทความที่ปฏิเสธ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ปฏิเสธ, จำนวนวันจากวันที่ส่งบทความถึงวันที่ปฏิเสธ, สถานะ ณ วันที่ปฏิเสธ และ เหตุผล',
  */
  // TODO: j.jid -> j.code

  $query = open_journal_get_query();

  $result = db_query("SELECT 
      j.jid `ID`,  
      j.code `Code`, 
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `Author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `Author (th)`, 
      j.address `Offices`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%m-%d %T') `Rejected at`,
      CEIL((status_changed - created)/(60*60*24)) `Duration (days)`, 
      s.name `State`, 
      j.reject_reason `Reason`  
    FROM 
      {open_journal_journal} j, 
      {open_journal_state} s 
    WHERE 
      j.sid = s.sid AND j.reject = 1 AND j.status_changed >= :start_date AND j.status_changed <= :end_date 
    ORDER BY
      j.status_changed DESC
  ", $query);

  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.5
function open_journal_report_journal_withdraw() {
  /*
  'title' => 'รายงานบทความที่ถอน',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ถอน, จำนวนวันจากวันที่ส่งบทความถึงวันที่ถอน, สถานะ ณ วันที่ถอน และ เหตุผล',
  */
  // TODO: j.jid -> j.code

  $query = open_journal_get_query();

  $result = db_query("SELECT 
      j.jid `ID`,  
      j.code `Code`, 
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `Author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `Author (th)`, 
      j.address `Offices`,
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%m-%d %T') `Withdraw at`,
      CEIL((status_changed - created)/(60*60*24)) `Duration (days)`, 
      s.name `State`, 
      j.withdraw_reason `Reason`  
    FROM 
      {open_journal_journal} j, 
      {open_journal_state} s 
    WHERE 
      j.sid = s.sid AND j.withdraw = 1 AND j.status_changed >= :start_date AND j.status_changed <= :end_date 
    ORDER BY
      j.status_changed DESC
  ", $query);

  open_journal_result_to_xlsx($result, __FUNCTION__);
}

// 5.8
function open_journal_report_journal_reviewer() {
  /*
  'title' => 'รายงานบทความ และผู้ทรงคุณวุฒิ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก และ รายชื่อผู้ทรงคุณวุฒิ',
  */
  // TODO: j.jid -> j.code

  $query = open_journal_get_query();

  $result = db_query("SELECT 
      j.jid `ID`,  
      j.code `Code`, 
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `Author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `Author (th)`, 
      j.address `Offices`,
      GROUP_CONCAT(u.uid SEPARATOR ',') `Reviewers` 
    FROM 
      {open_journal_journal} j 
    LEFT JOIN 
      (
        SELECT * 
        FROM 
          (
            SELECT jid, uid, status 
            FROM  {open_journal_journal_people} 
            WHERE role_list LIKE '%reviewer%' AND created >= :start_date AND created <= :end_date 
            ORDER BY created DESC
          ) jp 
        GROUP BY jp.jid, jp.uid 
        HAVING jp.status = 1
      ) jp 
    ON 
      j.jid = jp.jid 
    LEFT JOIN 
      {users} u 
    ON 
      jp.uid = u.uid 
    GROUP BY 
      jp.jid 
    HAVING 
      Reviewers IS NOT NULL
  ", $query)->fetchAll();

  foreach ($result as &$line) {
    $reviewers = array();
    foreach (explode(',', $line->Reviewers) as $uid) {
      $reviewer = user_load($uid);
      $reviewers[] = $reviewer->data['firstname'].' '.$reviewer->data['lastname'].' [uid:'.$reviewer->uid.']';
    }
    $line->Reviewers = implode(', ', $reviewers);
  }

  open_journal_result_to_xlsx($result, __FUNCTION__);
}

// 5.9
function open_journal_report_reviewer_journal() {
  /*
  'title' => 'รายงานผลการทำงานของผู้ทรงคุณวุฒิ',
  'subtitle' => 'แสดง : ชื่อผู้ทรงคุณวุฒิ จำนวนบทความที่ได้พิจารณา และรายชื่อบทความ',
  */
  // TODO: j.jid -> j.code

  $query = open_journal_get_query();
  $query[':uid'] = $_GET['uid'];

  $result = db_query("SELECT 
      jp.uid `Reviewer`,
      j.jid `ID`,  
      j.code `Code`, 
      DATE_FORMAT(FROM_UNIXTIME(jp.created), '%Y-%m-%d %T') `Review date`,
      j.title_en `Title (en)`, 
      j.title_sub `Title (th)`,
      j.address `Offices`
    FROM 
      (
        SELECT * 
        FROM 
          (
            SELECT jid, uid, status, created 
            FROM  {open_journal_journal_people} 
            WHERE role_list LIKE '%reviewer%' AND created >= :start_date AND created <= :end_date 
            ORDER BY created DESC
          ) jp 
        GROUP BY jp.jid, jp.uid 
        HAVING jp.status = 1
      ) jp 
    LEFT JOIN 
      {open_journal_journal} j 
    ON 
      jp.jid = j.jid 
    WHERE
      jp.uid = :uid
  ", $query)->fetchAll();

  foreach ($result as &$line) {
    $reviewer = user_load($line->Reviewer);

    $line->Reviewer = $reviewer->data['firstname'].' '.$reviewer->data['lastname'].' [uid:'.$reviewer->uid.']';
  }

  open_journal_result_to_xlsx($result, __FUNCTION__);

}
