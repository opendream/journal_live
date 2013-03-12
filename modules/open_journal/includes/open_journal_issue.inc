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

  $form['journal_wrapper'] = array(
    '#prefix' => '<div class="clearfix" id="poll-choice-wrapper">',
    '#suffix' => '</div>',
  );

  $form['journal_wrapper']['journals'] = array(
    '#prefix' => '<div id="journals-list">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'open_journal_issue_journals_form',
  );

  if (isset($form_state['values']['journals'])) {
    $issue['journals'] = $form_state['values']['journals'];
  }

  $delta = 0;
  $weight = 0;
  $additional_journal = 1;
  if (isset($issue['journals'])) {
    foreach ($issue['journals'] as $key => $journal) {
      $form['journal_wrapper']['journals'][$key]['journal'] = _open_journal_issue_journal_form($journal);
      $weight = max($journal['weight'], $weight);
    }
  }

  $existing_delta = $delta;
  for ($delta; $delta < $additional_journal; $delta++) {
    $weight++;
    $form['journal_wrapper']['journals'][$weight]['journal'] = _open_journal_issue_journal_form(array(
      'weight' => $weight,
    ));
  }

  $form['journal_wrapper']['more'] = array(
    '#type' => 'submit',
    '#value' => t('Add more journal'),
    '#weight' => 1,
    '#limit_validation_errors' => array(array('journals')),
    '#submit' => array('open_journal_issue_more_submit'),
    '#ajax' => array(
      'callback' => 'open_journal_issue_journals_js',
      'wrapper' => 'journals-list',
      'effect' => 'fade',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler to add more choices to a poll form.
 *
 * This handler is run regardless of whether JS is enabled or not. It makes
 * changes to the form state. If the button was clicked with JS disabled, then
 * the page is reloaded with the complete rebuilt form. If the button was
 * clicked with JS enabled, then ajax_form_callback() calls open_journal_issue_journals_js() to
 * return just the changed part of the form.
 */
function open_journal_issue_more_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback in response to new choices being added to the form.
 *
 * This returns the new page content to replace the page content made obsolete
 * by the form submission.
 *
 * @see open_journal_issue_more_submit()
 */
function open_journal_issue_journals_js($form, $form_state) {
  return $form['journal_wrapper']['journals'];
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
    if ($key === 'more') {
      continue;
    }

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

/**
 * View issue.
 */
function open_journal_issue_view($issue) {
  global $user;

  // Set title.
  drupal_set_title($issue->title);

  // List journals.
  $items = array();
  foreach ($issue->journals as $journal) {
    $items[] = l($journal->dc_title, 'journal/'. $journal->jid);
  }

  $output = theme('item_list', array('items' => $items));

  // Edit link.
  if ($user->uid == $issue->uid || $user->uid == 1) {
    $output .= l(t('Edit'), 'issue/'. $issue->issue_id .'/edit', array('query' => drupal_get_destination()));
  }
  return $output;
}