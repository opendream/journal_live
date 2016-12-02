<?php


function open_journal_template_trigger_on($name=NULL, $state=NULL) {


  $map = array(
    'template' => 'User select on dropdown',
    'auto' => 'State landing (system)',
    'approve' => 'Approve with message',
    'reject' => 'Reject with message',
    'time' => 'Timer of state configuration'
  );

  if ($state && $name) {
    if ($name == 'time' && $state['timeout_auto_reminder']) {
      $map['time'] = 'If state not change in '.$state['timeout_auto_reminder'].' days';
    }
    elseif ($name == 'reject' && $state['timeout_auto_reject']) {
      $map['reject'] = 'If state not change in '.$state['timeout_auto_reject'].' days and auto reject';
    }
  }
  
  if ($name) {
    return $map[$name];
  }
  
  return $map;
  
}

function open_journal_get_state_name($state) {
  // Check 2 level parent

  if (is_numeric($state)) {
    $state = db_query("SELECT name, sid, parent_sid, is_substate FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $state))->fetchAssoc();
  }

  $name = $state['name'];
  if ($state['parent_sid']) {
    $parent = db_query("SELECT name, sid, parent_sid FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $state['parent_sid']))->fetchObject();
    $name = $parent->name.' · '.$name ;
  }
  if ($parent->parent_sid) {
    $parent = db_query("SELECT name, sid, parent_sid FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $parent->parent_sid))->fetchObject();
    $name = $parent->name.' · '.$name ;
  }

  if ($state['is_substate']) {
    $name = $name . ' (second branch)';
  }

  return $name;
}

function open_journal_get_state_options($required=FALSE, $substate_only=FALSE) {

  if (!$required) {
    $state_options = array('' => '-');
  }

  foreach (open_journal_get_status_list(TRUE) as $state) {
    if ($substate_only && !$state['is_substate']) {
      continue;
    }
    $state_options[$state['sid']] = open_journal_get_state_name($state);
  }
  return $state_options;
}

function open_journal_template_list() {


  $header = array(t('Label'), t('Receivers'), t('Trigger on'), t('Message'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();

  $role_map = user_roles();
  
  foreach (open_journal_get_status_list(TRUE) as $state) {

    $state['name'] = '<div id="state-'.$state['sid'].'" class="state-anchor"></div><h2>'.open_journal_get_state_name($state).'</h2>';

    // Provide information of state

    $info = array();

    if ($state['actor_rid']) {
      $state['name'] .= '<strong> ♦ Actor · '.$role_map[$state['actor_rid']].'</strong>';
    }

    if ($state['description']) {
      $state['name'] .= '<p>'.nl2br($state['description']).'</p>';
    }


    if ($state['next_ssid']) {
      $info[] = 'Create new branch <a class="state-link" href="#state-'.$state['next_ssid'].'"><strong>'.open_journal_get_state_name($state['next_ssid']).'</strong></a>';
    }

    if (!$state['next_ssid'] && $state['upload_auto_approve']) {
      $next_sid = open_journal_next_sid($state['sid'], TRUE);
      $info[] = 'When detect a new file upload, system will change state to <a class="state-link" href="#state-'.$next_sid.'"><strong>'.open_journal_get_state_name($next_sid).'</strong></a> automatically!';
    }

    if (!$state['next_ssid'] && $state['timeout_auto_reminder']) {
      $time_template_links = array();
      $time_templates = db_query("SELECT sid, label, tpid FROM {open_journal_template} WHERE name = 'time' AND sid = :sid", array(':sid' => $state['sid']))->fetchAll();
      foreach ($time_templates as $time_template) {
        $time_template_links[] = '<a class="state-link" href="#template-'.$time_template->tpid.'"><strong>'.$time_template->label.'</strong></a>';
      }

      if (count($time_template_links)) {
        $info[] = 'When state not change in '.$state['timeout_auto_reminder'].' days, system will send mail with templates '.implode(', ', $time_template_links).' automatically!';
      }
    }

    if (!$state['next_ssid'] && $state['timeout_auto_approve']) {
      $next_sid = open_journal_next_sid($state['sid']);
      $info[] = 'When state not change in '.$state['timeout_auto_approve'].' days and @approve_by_timer_link has been visited, system will change state to <a class="state-link" href="#state-'.$next_sid.'"><strong>'.open_journal_get_state_name($next_sid).'</strong></a> automatically!';

    }

    if (!$state['next_ssid'] && $state['timeout_auto_reject']) {

      $reject_template_links = array();
      $reject_templates = db_query("SELECT sid, label, tpid FROM {open_journal_template} WHERE name = 'reject' AND sid = :sid", array(':sid' => $state['sid']))->fetchAll();

      foreach ($reject_templates as $reject_template) {
        $reject_template_links[] = '<a class="state-link" href="#template-'.$reject_template->tpid.'"><strong>'.$reject_template->label.'</strong></a>';
      }

      $with_templates = '';
      if (count($reject_template_links)) {
        $with_templates = 'send mail with templates '.implode(', ', $reject_template_links);
      }

      if ($state['timeout_auto_reject'] && $state['timeout_auto_approve']) {
        $info[] = 'When state not change in '.$state['timeout_auto_reject'].' days and @approve_by_timer_link has <u>not</u> been visited, system will reject article '.$with_templates.' automatically!';
      }
      else if ($state['timeout_auto_reject']) {
        $info[] = 'When state not change in '.$state['timeout_auto_reject'].' days, system will reject article '.$with_templates.' automatically!';

      }

    }


    if (count($info)) {
      $state['name'] .= theme('item_list', array('items' => $info));
    }

    // Templates of states

    $templates = db_query("SELECT t.*, GROUP_CONCAT(REPLACE(r.name, 'anonymous user', 'co-author') SEPARATOR ', ') roles FROM {open_journal_template} t LEFT JOIN {open_journal_template_role} tr ON t.tpid = tr.tpid LEFT JOIN {role} r ON tr.rid = r.rid WHERE t.sid = :sid GROUP BY t.tpid ORDER BY t.name = 'auto' DESC, t.name = 'template' DESC, t.name = 'time' DESC, t.name = 'approve' DESC, t.name = 'reject' DESC", array(':sid' => $state['sid']))->fetchAll();
    
    if (!count($templates)) {
      continue;
    }


    $row = array(
      array('data' => $state['name'], 'colspan' => 5),
      l(t('edit'), 'admin/config/open_journal/state/'. $state['sid']. '/edit', array('query' => drupal_get_destination()))
    );
    $rows[] = array('data' => $row, 'class' => array('row-state'));

    foreach ($templates as $template) {

      $message = '<p><h4>'.$template->template_title.'</h4></p>'.nl2br($template->body);
      $message = '<div class="state-anchor" id="template-'.$template->tpid.'"></div>'.$message;

      $row = array();
      $row[] = array('data' => $template->label? $template->label: '-', 'style' => 'min-width: 100px;');
      $row[] = nl2br($template->roles);
      $row[] = open_journal_template_trigger_on($template->name, $state);
      $row[] = $message;
      $row[] = l(t('edit'), 'admin/config/open_journal/template/'. $template->tpid. '/edit');
      $row[] = l(t('delete'), 'admin/config/open_journal/template/'. $template->tpid. '/delete');

      if ($template->color_code) {
        $row = array('data' => $row, 'style' => 'background-color: #'.$template->color_code);
      }

      $rows[] = $row;
    }

    if ($state['sid'] != open_journal_get_last_state()) {
      $rows[] = array(array('data' => '<div style="position: absolute; width: 100%; height: 100%; padding: 1px; top: 0; left: -1px; background-color: #FFF;"><div style="margin: 14px auto; width: 20px; font-size: 30px; color: #555;">·</div></div><br/><br/>', 'colspan' => 6, 'style' => 'position: relative;'));
    }
  }

  $output  = '<style type="text/css" media="all">
  table.tableheader-processed tr {
    border-top: solid 1px #000;
    border-width: 0 0 0 0;
    border-color: #aaa;
  }
  table.tableheader-processed tr{
    border-width: 1px 0 0 0;
  }
  tr.row-state {
    background-color: #c3c4bb;
  } 
  tr.even {
    
  }
  a.state-link {
    color: #000;
  }
  .state-anchor {
    margin-top: -100px;
    position: absolute;
  }
  </style>'.
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;

}

function open_journal_template_form($form, &$form_state, $template=NULL) {
  if ($template && $template->tpid) {
    drupal_set_title(t('Edit template'));
    $submit_text = t('Save changes');

    $form['tpid'] = array(
      '#type' => 'hidden',
      '#value' => $template->tpid,
      '#access' => FALSE,
    );
  }
  else {
    drupal_set_title(t('Add new template'));
    $submit_text = t('Create new template');
  }

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
  );

  $state_options = array();
  foreach (open_journal_get_status_list(TRUE) as $state) {
    $state_options[$state['sid']] = open_journal_get_state_name($state);
  }

  $form['info']['sid'] = array(
    '#type' => 'select',
    '#title' => t('Trigger state'),
    '#required' => TRUE,
    '#options' => open_journal_get_state_options(TRUE),
    '#default_value' => $template->sid,
  );
  $form['info']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#default_value' => $template->label,
  );
  $form['info']['name'] = array(
    '#type' => 'select',
    '#title' => t('Trigger on'),
    '#required' => TRUE,
    '#options' => open_journal_template_trigger_on(),
    '#default_value' => $template->name,
  );
  $form['info']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Dropdown name'),
    '#default_value' => $template->title,
    '#states' => array(
      'visible' => array(
        ':input[name="name"]' => array('value' => 'template'),
      ),
    ),
  );

  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Message'),
  );
  $form['message']['template_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#default_value' => $template->template_title,
  );
  $form['message']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#required' => TRUE,
    '#default_value' => $template->body,
  );

  $form['receiver'] = array(
    '#type' => 'fieldset',
    '#title' => t('Receiver'),
  );
  $roles_options = array(1 => 'anonymous user (co-author)');
  $roles_options += user_roles(TRUE);
  unset($roles_options[2]);
  $form['receiver']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => $roles_options,
    '#default_value' => $template? array_keys($template->roles): array(),
    '#description' => 'Unselect all for send this message to all roles'
  );

  $form['ui'] = array(
    '#type' => 'fieldset',
    '#title' => t('User interface'),
  );
  $form['ui']['color_code'] = array(
    '#type' => 'select',
    '#title' => t('Color'),
    '#options' => array(
      '' => '-',
      'F0E67A' => 'Yellow',
      'E6B0AA' => 'Red',
      'AED6F1' => 'Blue',
      'A9DFBF' => 'Green',
      'D7BDE2' => 'Purple',
      'F0B27A' => 'Orange',
    ),
    '#default_value' => $template->color_code,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_GET['destination']? $_GET['destination']: $_SERVER['HTTP_REFERER']),
  );


  return $form;;
}

function open_journal_template_form_submit($form, &$form_state) {
  $data = (object) $form_state['values'];
  $data->roles = array_filter($data->roles);

  open_journal_template_save($data);
  
  $is_new = (isset($data->tpid) && $data->tpid)? FALSE: TRUE;
  
  if ($is_new) {
    drupal_set_message(t('You tempalte has been created.'));
  }
  else {
    drupal_set_message(t('This tempalte has been updated.'));
  }
  
  drupal_goto('admin/config/open_journal/template', array('fragment' => 'template-'.$data->tpid));
}


function open_journal_template_delete_form($form, &$form_state, $template) {
  
  $form['tpid'] = array(
    '#type' => 'hidden',
    '#value' => $template->tpid,
    '#access' => FALSE
  );
  
  $title = t('Are you sure you want to delete the template %title?', array('%title' => $template->label));
  
  $form['title'] = array(
    '#markup' => '<h2>'.$title.'</h2>',
  );
  
  return confirm_form($form, $title, 'admin/config/open_journal/template/'.$template->tpid.'/edit');
}

function open_journal_template_delete_form_submit($form, &$form_state) {
  db_delete('open_journal_template')
    ->condition('tpid', $form_state['values']['tpid'])
    ->execute();
  drupal_set_message(t('The template has been deleted.'));
  drupal_goto('admin/config/open_journal/template');
}


function open_journal_state_form($form, &$form_state, $state=NULL) {

  if ($state && $state->sid !== NULL) {
    drupal_set_title(t('Edit state'));
    $submit_text = t('Save changes');

    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $state->sid,
      '#access' => FALSE,
    );
  }
  else {
    drupal_set_title(t('Add new state'));
    $submit_text = t('Create new state');
  }

  
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Information'),
  );
  $form['info']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $state->name,
    '#required' => TRUE,
  );
  $form['info']['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Code'),
    '#default_value' => $state->code,
    '#required' => TRUE,
  );
  $form['info']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $state->description,
  );

  $state_options = open_journal_get_state_options();
  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Network'),
  );
  $form['path']['next_sid'] = array(
    '#type' => 'select',
    '#title' => t('Next state'),
    '#options' => $state_options,
    '#default_value' => $state->next_sid,
  );
  $form['path']['parent_sid'] = array(
    '#type' => 'select',
    '#title' => t('Parent state'),
    '#options' => $state_options,
    '#default_value' => $state->parent_sid,
  );
  $form['path']['next_ssid'] = array(
    '#type' => 'select',
    '#title' => t('Next state (second branch)'),
    '#options' => open_journal_get_state_options(FALSE, TRUE),
    '#default_value' => $state->next_ssid,
  );
  $form['path']['is_substate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Is second branch state'),
    '#default_value' => $state->is_substate,
  );

  $form['event'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event/Timer'),
  );
  $form['event']['timeout_auto_reminder'] = array(
    '#type' => 'textfield',
    '#title' => t('Days of auto reminder (days)'),
    '#default_value' => $state->timeout_auto_reminder,
  );
  $form['event']['timeout_auto_approve'] = array(
    '#type' => 'textfield',
    '#title' => t('Days of auto change state to next state (days)'),
    '#default_value' => $state->timeout_auto_approve,
  );
  $form['event']['timeout_auto_reject'] = array(
    '#type' => 'textfield',
    '#title' => t('Days of auto reject (days)'),
    '#default_value' => $state->timeout_auto_approve,
  );
  $form['event']['upload_auto_approve'] = array(
    '#type' => 'checkbox',
    '#title' => t('Detect a new file upload and change state to next state ?'),
    '#default_value' => $state->upload_auto_approve,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), $_GET['destination']? $_GET['destination']: $_SERVER['HTTP_REFERER']),
  );


  return $form;
}

function open_journal_state_form_submit($form, &$form_state) {
  $data = (object) $form_state['values'];

  open_journal_state_save($data);
  
  $is_new = (isset($data->sid) && $data->sid)? FALSE: TRUE;
  
  if ($is_new) {
    drupal_set_message(t('You state has been created.'));
  }
  else {
    drupal_set_message(t('This state has been updated.'));
  }
  
  drupal_goto('admin/config/open_journal/template');
}


function open_journal_state_delete_form($form, &$form_state, $state) {
  
  $form['sid'] = array(
    '#type' => 'hidden',
    '#value' => $state->sid,
    '#access' => FALSE
  );
  
  $title = t('Are you sure you want to delete the state %title?', array('%title' => $state->name));
  
  $form['title'] = array(
    '#markup' => '<h2>'.$title.'</h2>',
  );
  
  return confirm_form($form, $title, 'admin/config/open_journal/state/'.$state->sid.'/edit');
}

function open_journal_state_delete_form_submit($form, &$form_state) {
  db_delete('open_journal_state')
    ->condition('sid', $form_state['values']['sid'])
    ->execute();
  drupal_set_message(t('The state has been deleted.'));
  drupal_goto('admin/config/open_journal/template');
}