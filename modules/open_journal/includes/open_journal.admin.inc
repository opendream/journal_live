<?php


function open_journal_template_list() {


  $header = array(t('Title'), t('Body'), t('Roles'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();


  foreach (open_journal_get_status_list(TRUE) as $state) {

    if ($state['parent_sid']) {
      $parent_name = db_query("SELECT name FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $state['parent_sid']))->fetchField();
      $state['name'] = $parent_name.' → '.$state['name'];
     }

  $templates = db_query("SELECT t.*, GROUP_CONCAT(r.name SEPARATOR ', ') roles FROM {open_journal_template} t LEFT JOIN {open_journal_template_role} tr ON t.tpid = tr.tpid LEFT JOIN {role} r ON tr.rid = r.rid WHERE t.sid = :sid GROUP BY t.tpid", array(':sid' => $state['sid']))->fetchAll();
  
  if (!count($templates)) {
    continue;
  }

  $row = array(array('data' => '<h2>'.$state['name'].'</h2>', 'colspan' => 5));
  $rows[] = array('data' => $row, 'class' => array('row-state'));

  foreach ($templates as $template) {
    $row = array();
    $row[] = $template->title;
    $row[] = nl2br($template->body);
    $row[] = nl2br($template->roles);
    $row[] = l(t('edit'), 'admin/config/open_journal/template/'. $template->tpid. '/edit', array('query' => drupal_get_destination()));
    $row[] = l(t('delete'), 'admin/config/open_journal/template/'. $template->tpid. '/delete');

    $rows[] = $row;
  }

  }

  return theme('table', array('header' => $header, 'rows' => $rows));

}

function open_journal_template_form($form, &$form_state, $template) {
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

    if ($state['parent_sid']) {
      $parent_name = db_query("SELECT name FROM {open_journal_state} WHERE sid = :sid", array(':sid' => $state['parent_sid']))->fetchField();
      $state['name'] = $parent_name.' → '.$state['name'];
    }
    $state_options[$state['sid']] = $state['name'];
  }

  $form['info']['sid'] = array(
    '#type' => 'select',
    '#title' => t('Trigger state'),
    '#required' => TRUE,
    '#options' => $state_options,
    '#default_value' => $template->sid,
  );
  $form['info']['name'] = array(
    '#type' => 'select',
    '#title' => t('Trigger on'),
    '#required' => TRUE,
    '#options' => array(
      'template' => 'User select on dropdown',
      'auto' => 'State change (system)',
      'approve' => 'Approve with message',
      'reject' => 'Override reject reason',
      'time' => 'Timer of state configuration'
    ),
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
  $roles_options = array(1 => 'co-author');
  $roles_options += user_roles(TRUE);
  unset($roles_options[2]);
  $form['receiver']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => $roles_options,
    '#default_value' => array_keys($template->roles),
    '#description' => 'Unselect all for send this message to all roles'
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
  
  drupal_goto('admin/config/open_journal/template');
}