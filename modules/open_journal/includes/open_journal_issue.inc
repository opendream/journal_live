<?php

/**
 * Issue form.
 */
function open_journal_issue_form($form, $form_state, $issue = NULL) {
  $form_state['cache'] = TRUE;

  $form['#attached']['css'][] = drupal_get_path('module', 'open_journal') . '/css/open_journal_issue.css';

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Issue title'),
    '#required' => TRUE,
    '#default_value' => isset($issue) ? $issue->title : '',
    '#weight' => -5,
  );

  if (isset($form_state['section_count'])) {
    $section_count = $form_state['section_count'];
  }
  else {
    $section_count = max(2, empty($issue->sections) ? 2 : count($issue->sections));
  }

  $form['sections_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -4,
    '#prefix' => '<div class="clearfix" id="sections-wrapper">',
    '#suffix' => '</div>',
  );

  $form['sections_wrapper']['sections'] = array(
    '#prefix' => '<div id="issue-sections">',
    '#suffix' => '</div>',
    '#theme' => 'open_journal_issue_sections_form',
  );

  $delta = 0;
  $weight = 0;

  $sections = array();
  if (isset($form_state['sections'])) {
    $sections = $form_state['sections'];
  }
  elseif (isset($issue->sections)) {
    $sections = $issue->sections;
  }

  if (!empty($sections)) {
    $delta = count($sections);
    foreach ($sections as $id => $section) {
      // $key = 'id:'. $id;
      $key = $id;
      $form['sections_wrapper']['sections'][$key] = _issue_sections($key, $section['id'], $section['title'], $section['weight'], $section_count, $section['journals']);
      $weight = max($section['weight'], $weight);
    }
  }

  // More input.
  $existing_delta = $delta;
  for ($delta; $delta < $section_count; $delta++) {
    // $key = 'new:' . ($delta - $existing_delta);
    $key = $delta;
    $weight++;
    $form['sections_wrapper']['sections'][$key] = _issue_sections($key, NULL, '', $weight, $section_count);
  }

  $form['sections_wrapper']['section_more'] = array(
    '#type' => 'submit',
    '#value' => t('More sections'),
    '#attributes' => array(
      'title' => t("If the amount of boxes above isn't enough, click here to add more sections."),
    ),
    '#weight' => 1,
    '#limit_validation_errors' => array(array('sections')),
    '#submit' => array('open_journal_issue_sections_more_submit'),
    '#ajax' => array(
      'callback' => 'open_journal_issue_sections_js',
      'wrapper' => 'issue-sections',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Section row form.
 */
function _issue_sections($key, $id = NULL, $value = '', $weight = 0, $size = 10, $journals = array()) {
  $form = array(
    '#tree' => TRUE,
    '#weight' => $weight,
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
    '#parents' => array('sections', $key, 'id'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => $value !== '' ? t('Section title') : t('New section title'),
    '#title_display' => 'invisible',
    '#default_value' => $value,
    '#parents' => array('sections', $key, 'title'),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => $value !== '' ? t('Weight for section @label', array('@label' => $value)) : t('Weight for new section'),
    '#title_display' => 'invisible',
    '#default_value' => $weight,
    '#delta' => $size,
    '#parents' => array('sections', $key, 'weight'),
  );

  $form['journals_wrapper'] = array(
    '#weight' => -4,
    '#prefix' => '<div class="clearfix issue-journals-wrapper" id="journals-wrapper-'. $weight .'">',
    '#suffix' => '</div>',
  );

  $form['journals_wrapper']['journals'] = array(
    '#prefix' => '<div id="issue-journals-'. $key .'">',
    '#suffix' => '</div>',
    '#theme' => 'open_journal_issue_journals_form',
    '#section_key' => $key,
  );

  // Calculate number to display.
  $journal_count = max(2, empty($journals) ? 2 : count($journals) + 1);

  $delta = 0;
  $_weight = 0;
  if ($journals) {
    $delta = count($journals);
    foreach ($journals as $id => $journal) {
      // $_key = 'id:' . $id;
      $_key = $id;
      $form['journals_wrapper']['journals'][$_key] = _issue_journals($key, $_key, $journal['id'], $journal['title'], $journal['weight'], $journal_count);
      $_weight = max($journal['weight'], $_weight);
    }
  }

  $existing_delta = $delta;
  for ($delta; $delta < $journal_count; $delta++) {
    // $_key = 'new:' . ($delta - $existing_delta);
    $_key = $delta;
    $_weight++;
    $form['journals_wrapper']['journals'][$_key] = _issue_journals($key, $_key, NULL, '', $_weight, $journal_count);
  }

  $more_name = 'more_'. $key .'_journal_button';
  $form['journals_wrapper']['journal_more'] = array(
    '#id' => str_replace('_', '-', $more_name),
    '#name' => $more_name,
    '#type' => 'submit',
    '#value' => t('More journals'),
    '#attributes' => array(
      'title' => t("If the amount of boxes above isn't enough, click here to add more journals."),
    ),
    '#weight' => 1,
    '#limit_validation_errors' => array(array('sections')),
    '#submit' => array('open_journal_issue_journals_more_submit'),
    '#ajax' => array(
      'callback' => 'open_journal_issue_journals_js',
      'wrapper' => 'issue-journals-'. $key,
      'effect' => 'fade',
    ),
  );

  return $form;
}

/**
 * Journal row form.
 */
function _issue_journals($section_key, $key, $id = NULL, $value = '', $weight = 0, $size = 10) {
  $form = array(
    '#tree' => TRUE,
    '#weight' => $weight,
  );

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
    '#parents' => array('sections', $section_key, 'journals', $key, 'id'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => $value !== '' ? t('Journal title') : t('New journal title'),
    '#title_display' => 'invisible',
    '#default_value' => $value,
    '#autocomplete_path' => OPEN_JOURNAL_PREFIX_PATH .'/autocomplete',
    '#parents' => array('sections', $section_key, 'journals', $key, 'title'),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => $value !== '' ? t('Weight for journal @label', array('@label' => $value)) : t('Weight for new journal'),
    '#title_display' => 'invisible',
    '#default_value' => $weight,
    '#delta' => $size,
    '#parents' => array('sections', $section_key, 'journals', $key, 'weight'),
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
function open_journal_issue_sections_more_submit($form, &$form_state) {
  if ($form_state['values']['section_more']) {
    $n = $_GET['q'] == 'system/ajax' ? 1 : 5;
    $form_state['section_count'] = count($form_state['values']['sections']) + $n;
  }
  
  $form_state['sections'] = array_values($form_state['values']['sections']);

  // Not remember element data.
  unset($form_state['input']['sections']);

  // Rebuilt form.
  $form_state['rebuild'] = TRUE;
}

function open_journal_issue_journals_more_submit($form, &$form_state) {
  $input = $form_state['input'];
  $element_name = $input['_triggering_element_name'];

  if ($form_state['values'][$element_name]) {
    $button = explode('_', $input['_triggering_element_name']);

    // $n = $_GET['q'] == 'system/ajax' ? 1 : 5;
    // $form_state['journal_count'][$key] = count($form_state['values']['sections'][$key]['journals']) + $n;

    $form_state['sections'] = array_values($form_state['values']['sections']);
  }

  // Not remember element data.
  unset($form_state['input']['sections']);

  // Rebuilt form.
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
function open_journal_issue_sections_js($form, $form_state) {
  return $form['sections_wrapper']['sections'];
}

function open_journal_issue_journals_js($form, &$form_state) {
  if ($form_state['input']['_triggering_element_name'] !== 'op') {
    $button = explode('_', $form_state['input']['_triggering_element_name']);
    $key = $form_state['section_id'];
    return $form['sections_wrapper']['sections'][$button[1]]['journals_wrapper']['journals'];
  }
}

/**
 * Issue form validate.
 */
function open_journal_issue_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Check journal pattern and journal is exists.
  $pattern = '/^(?:\s*|(.*) )?\[\s*id\s*:\s*(\d+)\s*\]$/';

  foreach ($values['sections'] as $delta => $section) {
    $empty_journal = TRUE;
    $deltas = array();
    foreach ($section['journals'] as $delta2 => $journal) {
      $journal_title = trim($journal['title']);

      if (empty($deltas)) {
        $deltas = array($delta, $delta2);
      }

      if (empty($journal_title)) {
        unset($form_state['values']['sections'][$delta]['journals'][$delta2]);
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

          if (trim($title) === trim($real_title)) {
            $empty_journal = FALSE;
            $form_state['values']['sections'][$delta]['journals'][$delta2]['jid'] = $jid;
          }
          else {
            form_set_error('sections]['. $delta .'][journals]['. $delta2 .'][title', t('Journal not found. Please choose from autocomplete again.'));
          }
        }
      }
      else {
        form_set_error('sections]['. $delta .'][journals]['. $delta2 .'][title', t('Journal not found. Please choose from autocomplete again.'));
      }
    }


    $section_title = trim($section['title']);
    if (empty($section_title) && !$empty_journal) {
      form_set_error('sections]['. $delta .'][title', t('Please enter section title'));
    }

    if (!empty($section_title) && $empty_journal && $deltas) {
      form_set_error('sections]['. $deltas[0] .'][journals][' .$deltas[1] .'][title', t('ddd'));
    }
  }
}

/**
 * Issue form submit.
 */
function open_journal_issue_form_submit($form, $form_state) {
  global $user;
  $values = $form_state['values'];
  dpm($values);

  // $issue_id = db_insert('open_journal_issue')
  //   ->fields(array(
  //     'title' => $values['title'],
  //     'uid' => $user->uid,
  //     'status' => 1,
  //     'created' => time(),
  //     'changed' => time(),
  //   ))
  //   ->execute();

  // if ($issue_id) {
  //   foreach ($values['journals'] as $journal) {
  //     if (!isset($journal['jid'])) {
  //       continue;
  //     }

  //     db_insert('open_journal_issue_journal')
  //       ->fields(array(
  //         'issue_id' => $issue_id,
  //         'jid' => $journal['jid'],
  //         'weight' => $journal['weight'],
  //       ))
  //       ->execute();
  //   }
  // }
}

/**
 * Issue add form.
 */
function open_journal_issue_add() {
  $form = drupal_get_form('open_journal_issue_form');
  return $form;
}

/**
 * Issue edit form.
 */
function open_journal_issue_edit($issue) {
  $form = drupal_get_form('open_journal_issue_form', $issue);
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

/**
 * Returns HTML for issue sections.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_open_journal_issue_sections_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('issue-sections-form-table', 'order', 'sibling', 'section-weight');

  $rows = array();
  $headers = array('', t('Section'), t('Weight'));

  foreach (element_children($form) as $key) {
    if ($key === 'section_more') {
      continue;
    }

    $form[$key]['weight']['#attributes']['class'] = array('section-weight');
    
    $form[$key]['title']['#prefix'] = '<div class="title-remove-wrapper">';
    $form[$key]['remove']['#suffix'] = '</div>';

    $row = array();
    $row['data'] = array();
    $row['data'][] = array('class' => array('section-flag'));
    $row['data'][] = drupal_render($form[$key]['title']) . drupal_render($form[$key]['remove']) . drupal_render($form[$key]['journals_wrapper']);
    $row['data'][] = drupal_render($form[$key]['weight']);
    $row['class'] = array('draggable');

    $rows[] = $row;
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array('id' => 'issue-sections-form-table')
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Returns HTML for issue journals in sections.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_open_journal_issue_journals_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('issue-journals-form-table-'. $form['#section_key'], 'order', 'sibling', 'journal-weight');

  $rows = array();
  // $headers = array(t('Journal'), t('Remove'), t('Weight'));
  $headers = array(t('Journal'), t('Weight'));

  foreach (element_children($form) as $key) {
    if ($key === 'more') {
      continue;
    }

    $form[$key]['weight']['#attributes']['class'] = array('journal-weight');

    $row = array();
    $row['data'] = array();
    $row['data'][] = drupal_render($form[$key]['title']);
    // $row['data'][] = drupal_render($form[$key]['remove']);
    $row['data'][] = drupal_render($form[$key]['weight']);
    $row['class'] = array('draggable');

    $rows[] = $row;
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array('id' => 'issue-journals-form-table-'. $form['#section_key']),
  ));
  $output .= drupal_render_children($form);
  return $output;
}