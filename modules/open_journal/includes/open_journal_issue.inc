<?php

/**
 * Issue form.
 */
function open_journal_issue_form($form, $form_state, $issue = NULL) {
  $form_state['cache'] = TRUE;
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Issue title'),
    '#required' => TRUE,
    '#default_value' => isset($issue['title']) ? $issue['title'] : '',
  );

  // $form['journal'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Journal'),
  //   '#autocomplete_path' => OPEN_JOURNAL_PREFIX_PATH .'/autocomplete',
  // );

  $form['journals'] = array(
    '#prefix' => '<div id="journals-list">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'open_journal_issue_journals_form',
  );

  $delta = 0;
  $weight = 0;
  $additional_journal = 2;
  if (isset($issue['journals'])) {
    foreach ($issue['journals'] as $key => $journal) {
      $form['journals'][$key]['journal'] = _open_journal_issue_journal_form($journal);
      $weight = max($journal['weight'], $weight);
    }
    $additional_journal = 1;
  }

  $existing_delta = $delta;
  for ($delta; $delta < $additional_journal; $delta++) {
    $weight++;
    $form['journals'][$weight]['journal'] = _open_journal_issue_journal_form(array(
      'weight' => $weight,
    ));
  }

  // $form['journal_wrapper'] = array(
  //   '#tree' => FALSE,
  //   '#prefix' => '<div class="clearfix" id="journal-wrapper">',
  //   '#suffix' => '</div>',
  // );

  // // Container for just the poll choices.
  // $form['journal_wrapper']['journals'] = array(
  //   '#prefix' => '<div id="journals-list">',
  //   '#suffix' => '</div>',
  //   '#theme' => 'open_journal_issue_journals_list',
  // );

  // $delta = 0;
  // $weight = 0;

  // $existing_delta = $delta;
  // for ($delta; $delta < 2; $delta++) {
  //   $key = 'new:' . ($delta - $existing_delta);
  //   // Increase the weight of each new choice.
  //   $weight++;
  //   $form['journal_wrapper']['journals'][$key] = _open_journal_issue_journal_form($key, '', '', $weight);
  // }

  // $form['journal_wrapper']['more'] = array(
  //   '#type' => 'submit',
  //   '#value' => t('Add more journal'),
  //   '#attributes' => array(
  //     'title' => t("If the amount of boxes above isn't enough, click here to add more journal."),
  //   ),
  //   '#weight' => 1,
  //   '#limit_validation_errors' => array(array('journals')),
  //   '#submit' => array('poll_more_choices_submit'),
  //   '#ajax' => array(
  //     'callback' => 'open_journal_issue_journals_js',
  //     'wrapper' => 'journals-list',
  //     'effect' => 'fade',
  //   ),
  // );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Issue form validate.
 */
function open_journal_issue_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Check journal pattern and journal is exists.
  $pattern = '/^(?:\s*|(.*) )?\[\s*id\s*:\s*(\d+)\s*\]$/';
  foreach ($values['journals'] as $delta => $journal) {
    $journal_title = trim($journal['title']);

    if (empty($journal_title)) {
      unset($form_state['values']['journals'][$delta]);
      continue;
    }

    preg_match($pattern, $journal_title, $matches);

    if (!empty($matches)) {
      // Explicit nid. Check that the 'title' part matches the actual title for
      // the nid.
      list(, $title, $jid) = $matches;
      if (!empty($title)) {
        $real_title = db_select('open_journal_journal', 'j')
          ->fields('j', array('dc_title'))
          ->condition('j.jid', $jid)
          ->execute()
          ->fetchField();
        if (trim($title) != trim($real_title)) {
          form_set_error('journals]['. $delta .'][title', 'xxxx');
        }
      }
      $form_state['values']['journals'][$delta]['jid'] = $jid;
    }
  }
}

/**
 * Issue form submit.
 */
function open_journal_issue_form_submit($form, $form_state) {
  global $user;
  $values = $form_state['values'];

  $issue_id = db_insert('open_journal_issue')
    ->fields(array(
      'title' => $values['title'],
      'uid' => $user->uid,
      'status' => 1,
      'created' => time(),
      'changed' => time(),
    ))
    ->execute();

  if ($issue_id) {
    dpm($values['journals']);
    foreach ($values['journals'] as $journal) {
      if (!isset($journal['jid'])) {
        continue;
      }

      db_insert('open_journal_issue_journal')
        ->fields(array(
          'issue_id' => $issue_id,
          'jid' => $journal['jid'],
          'weight' => $journal['weight'],
        ))
        ->execute();
    }
  }
}

/**
 * 
 */
function _open_journal_issue_journal_form($journal) {
  $form = array(
    '#tree' => TRUE,
    '#weight' => $journal['weight'],
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#title_display' => 'invisible',
    '#autocomplete_path' => OPEN_JOURNAL_PREFIX_PATH .'/autocomplete',
    '#default_value' => isset($journal['title']) ? $journal['title'] : '',
    '#parents' => array('journals', $journal['weight'], 'title'),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title_display' => 'invisible',
    '#default_value' => isset($journal['weight']) ? $journal['weight'] : '',
    '#parents' => array('journals', $journal['weight'], 'weight'),
    '#attributes' => array(
      'class' => array('journals-weight'),
    ),
  );

  return $form;
}

/**
 * Returns HTML for an admin poll form for choices.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_open_journal_issue_journals_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('journals-form-table', 'order', 'sibling', 'journals-weight');

  $delta = 0;
  $rows = array();
  $headers = array(t('Journal'), t('Weight'));

  foreach (element_children($form) as $key) {
    $delta++;

    $row = array();
    $row['data'] = array();
    $row['data'][] = drupal_render($form[$key]['journal']['title']);
    $row['data'][] = drupal_render($form[$key]['journal']['weight']);
    $row['class'] = array('draggable');

    $rows[] = $row;
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array('id' => 'journals-form-table')
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Issue add form.
 */
function open_journal_issue_add() {
  $form = drupal_get_form('open_journal_issue_form');
  return $form;
}
