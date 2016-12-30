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
    //5.3+
    array(
      'title' => 'รายงานบทความที่ตีพิมพ์ และผู้เขียนร่วม',
      'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ผู้เขียนร่วม, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-publication2',
      'callback' => 'open_journal_report_journal_publication2'
    ),
    //5.+
    array(
      'title' => 'รายงานบทความ และช่วงเวลาในแต่ละสถานะ',
      'subtitle' => 'แสดง : ID, ชื่อบทความ และ ช่วงเวลาที่อยู่ในแต่ละสถานะ', 
      'url' => OPEN_JOURNAL_PREFIX_PATH.'/report/journal-days-in-status',
      'callback' => 'open_journal_report_journal_days_in_status'
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

function open_journal_result_to_xlsx2($result, $filename) {

  include_once("PHPExcel.php");

  $filename = str_replace(array('open_journal_', '_'), array('', '-'), $filename);

  $filename = open_journal_get_filename($filename).".".date("Y-m-d-His", REQUEST_TIME).".xlsx";
  header('Content-disposition: attachment; filename="'.$filename.'"');
  header("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet; charset=utf-8");
  header('Content-Transfer-Encoding: binary');
  header('Cache-Control: must-revalidate');
  header('Pragma: public');

  $objPHPExcel = new PHPExcel();

  $objPHPExcel->getProperties()->setCreator("E-Journal")
               ->setLastModifiedBy("E-Journal")
               ->setTitle("Office 2007 XLSX Test Document")
               ->setSubject("Office 2007 XLSX Test Document")
               ->setDescription("Test document for Office 2007 XLSX, generated using PHP classes.")
               ->setKeywords("office 2007 openxml php")
               ->setCategory("Test result file");

  // Add some data
$objPHPExcel->setActiveSheetIndex(0)
            ->setCellValue('A1', 'Hello')
            ->setCellValue('B2', 'world!')
            ->setCellValue('C1', 'Hello')
            ->setCellValue('D2', 'world!');

$objPHPExcel->getActiveSheet()->setTitle('Simple');

$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
$objWriter->save('php://output');
drupal_exit();

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

function open_journal_result_to_xlsx($result, $filename) {

  include_once("xlsxwriter.class.php");

  $filename = str_replace(array('open_journal_', '_'), array('', '-'), $filename);

  $filename = open_journal_get_filename($filename).".".date("Y-m-d-His", REQUEST_TIME).".xlsx";
  header('Content-disposition: attachment; filename="'.XLSXWriter::sanitize_filename($filename).'"');
  header("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet; charset=utf-8");
  header('Content-Transfer-Encoding: binary');
  header('Cache-Control: must-revalidate');
  header('Pragma: public');


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

  $meta = 'as of : '.date("Y-m-d H:i:s", REQUEST_TIME);

  $writer = new XLSXWriter();
  $writer->writeSheet($data, 'Sheet1', $header, $meta);
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
  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      s.name `status`, 
      j.code `ID`, 
      j.jid `URL ID`, 
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      j.address `offices`,
      DATE_FORMAT(FROM_UNIXTIME(j.created), '%Y-%M-%d') `submission date`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%M-%d') `date of latest status change`, 
      CEIL((j.status_changed - j.created)/(60*60*24)) `days in current status` 
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
      $cols .= "IFNULL(SUM(j.sid = :last_sid AND DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y') = '".$year->year."'), 0) `published in ".$year->year."`, ";
    }
  }

  $result = db_query("SELECT 
      s.name `status`, 
      $cols
      IFNULL(COUNT(j.jid) - SUM(j.sid = :last_sid), 0) `unpublished`, 
      COUNT(j.jid) `total`
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

  $result = $result->fetchAll();
  $total_row = array();
  foreach ($result as $i => $obj) {
    foreach (get_object_vars($obj) as $key => $value) {
      if (is_numeric($obj->{$key})) {
        $total_row[$key] += $obj->{$key};
      }
      else {
        $total_row[$key] = 'Total';
      }
    }
  }
  $result[] = (object) $total_row;


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

  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%M-%d') `publish date`, 
      j.code `ID`, 
      j.jid `URL ID`, 
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `author (th)`, 
      j.address `offices`
    FROM 
      {open_journal_journal} j  
    WHERE 
      j.sid = :last_sid AND j.reject = 0 AND j.status_changed >= :start_date AND j.status_changed <= :end_date 
    ORDER BY
      j.status_changed DESC
  ", $query);

  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.3+
function open_journal_report_journal_publication2() {
  /*
  'title' => 'รายงานบทความที่ตีพิมพ์',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, ฉบับที่ตีพิมพ์ และ วันเดือนปีที่ตีพิมพ์', 
  */
  // TODO: j.jid -> j.code, WHERE j.withdraw = 0
  $query = open_journal_get_query();
  $query[':last_sid'] = open_journal_get_last_state();

  db_query("SET @rn=0");
  $tmp_result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%M-%d') `publish date`, 
      j.code `ID`, 
      j.jid `URL ID`, 
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      j.creator_en `author (en)`, 
      j.creator_sub `author (th)`, 
      j.address `offices`
    FROM 
      {open_journal_journal} j  
    WHERE 
      j.sid = :last_sid AND j.reject = 0 AND j.status_changed >= :start_date AND j.status_changed <= :end_date 
    ORDER BY
      j.status_changed DESC
  ", $query);

  $result = array();
  foreach ($tmp_result as $row) {
    $crow = clone $row;
    $author_en_list = explode(',', $crow->{'author (en)'});
    $author_sub_list = explode(',', $crow->{'author (th)'});

    if (count($author_en_list) >= count($author_th_list)) {
      $author_list = $author_en_list;
    }
    else {
      $author_list = $author_sub_list;
    }

    $is_first = TRUE;
    foreach ($author_list as $i => $author) {
      if (!trim($author)) {
        continue;
      }

      $crow = clone $row;
      if (!$is_first) {
        foreach (get_object_vars($crow) as $key => $type) {
          $crow->{$key} = '';
        }
      }
      $crow->{'author (en)'} = $author_en_list[$i];
      $crow->{'author (th)'} = $author_sub_list[$i];
      $result[] = $crow;
      $is_first = FALSE;
    }

  }

  open_journal_result_to_xlsx($result, __FUNCTION__);

}

// 5.+
function open_journal_report_journal_days_in_status() {

  $query = open_journal_get_query();


  $status_list = array();
  foreach (open_journal_get_status_list(TRUE) as $sid => $status) {
    $status_list[] = $status['name'];
  }

  $cols = "";
  if(count($status_list) > 0) {
    foreach ($status_list as $name) {
      $cols .= ", 0 AS `$name`";
    }
  }

  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      j.code `ID`, 
      j.jid `URL ID`, 
      j.title_en `title (en)`, 
      j.title_sub `title (th)`,
      s.name `status`
      $cols
    FROM
      {open_journal_journal} j
    LEFT JOIN
      {open_journal_state} s
    ON
      j.sid = s.sid 
    WHERE 
      j.reject = 0 AND j.withdraw = 0 AND j.created >= :start_date AND j.created <= :end_date
  ", $query)->fetchAll();

  $journal_list = array();
  foreach ($result as $row) {
    $journal_list[$row->{'URL ID'}] = $row; 
  }

  $query[':last_sid'] = open_journal_get_last_state();

  $now = REQUEST_TIME;
  $result_value = db_query("SELECT 
      js.jid jid, 
      s.name name, 
      SUM(
        IF(
          js.sid = :last_sid,
          CEIL(
            ($now-js.start_date)/(60*60*24)
          ),
          CEIL(
            (IFNULL(js.end_date, $now)-js.start_date)/(60*60*24)
          )
        )
      ) days 
    FROM 
      open_journal_journal_state js 
    LEFT JOIN 
      open_journal_state s 
    ON 
      js.sid = s.sid 
    LEFT JOIN 
      open_journal_journal j 
    ON 
      js.jid =j.jid 
    WHERE 
      j.reject = 0 AND j.withdraw = 0 AND j.created >= :start_date AND j.created <= :end_date 
    GROUP BY 
      js.jid, s.name
  ", $query);

  foreach ($result_value as $value) {
    $journal_list[$value->jid]->{$value->name} += $value->days;
  }

  open_journal_result_to_xlsx($journal_list, __FUNCTION__);

}

// 5.4
function open_journal_report_journal_reject() {
  /*
  'title' => 'รายงานบทความที่ปฏิเสธ',
  'subtitle' => 'แสดง : ID, ชื่อบทความ, ผู้เขียนหลัก, วันที่ปฏิเสธ, จำนวนวันจากวันที่ส่งบทความถึงวันที่ปฏิเสธ, สถานะ ณ วันที่ปฏิเสธ และ เหตุผล',
  */
  // TODO: j.jid -> j.code

  $query = open_journal_get_query();

  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      DATE_FORMAT(FROM_UNIXTIME(j.created), '%Y-%M-%d') `submission date`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%M-%d') `reject date`,
      CEIL((status_changed - created)/(60*60*24)) `days in process`, 
      s.name `status before rejecting`, 
      j.code `ID`, 
      j.jid `URL ID`,  
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `author (th)`, 
      j.address `offices`, 
      j.reject_reason `reason`  
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

  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      DATE_FORMAT(FROM_UNIXTIME(j.created), '%Y-%M-%d') `submission date`, 
      DATE_FORMAT(FROM_UNIXTIME(j.status_changed), '%Y-%M-%d') `withdraw date`,
      CEIL((status_changed - created)/(60*60*24)) `days in process`, 
      s.name `status before withdrawal`,
      j.code `ID`, 
      j.jid `URL ID`,  
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `author (th)`, 
      j.address `offices`,
      j.withdraw_reason `reason`  
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

  db_query("SET @rn=0");
  $result = db_query("SELECT 
      @rn:=@rn+1 AS `no.`, 
      j.code `ID`, 
      j.jid `URL ID`,  
      j.title_en `title (en)`, 
      j.title_sub `title (th)`, 
      SUBSTRING_INDEX(j.creator_en, ',', 1) `author (en)`, 
      SUBSTRING_INDEX(j.creator_sub, ',', 1) `author (th)`, 
      j.address `offices`,
      GROUP_CONCAT(u.uid SEPARATOR ',') `reviewers` 
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
    foreach (explode(',', $line->reviewers) as $uid) {
      $reviewer = user_load($uid);
      $reviewers[] = $reviewer->data['firstname'].' '.$reviewer->data['lastname'].' [uid:'.$reviewer->uid.']';
    }
    $line->reviewers = implode(', ', $reviewers);
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
      jp.uid `reviewer`,
      j.code `ID`, 
      j.jid `URL ID`,  
      DATE_FORMAT(FROM_UNIXTIME(jp.created), '%Y-%M-%d') `review date`,
      j.title_en `title (en)`, 
      j.title_sub `title (th)`,
      j.address `offices`
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
    $reviewer = user_load($line->reviewer);

    $line->reviewer = $reviewer->data['firstname'].' '.$reviewer->data['lastname'].' [uid:'.$reviewer->uid.']';
  }

  open_journal_result_to_xlsx($result, __FUNCTION__);

}
