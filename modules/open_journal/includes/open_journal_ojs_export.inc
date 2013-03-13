<?php
function generate_xml_element( $dom, $data ) {
    if ( empty( $data['name'] ) )
        return false;

    // Create the element
    $element_value = ( ! empty( $data['value'] ) ) ? $data['value'] : null;
    $element = $dom->createElement( $data['name'], $element_value );

    // Add any attributes
    if ( ! empty( $data['attributes'] ) && is_array( $data['attributes'] ) ) {
        foreach ( $data['attributes'] as $attribute_key => $attribute_value ) {
            $element->setAttribute( $attribute_key, $attribute_value );
        }
    }

    // Any other items in the data array should be child elements
    foreach ( $data as $data_key => $child_data ) {
        if ( ! is_numeric( $data_key ) )
            continue;

        $child = generate_xml_element( $dom, $child_data );
        if ( $child )
            $element->appendChild( $child );
    }

    return $element;
}

function generate_author( $author ){
  if( !isset($author->data['firstname']) ) {
    $author->data['firstname'] = $author->name;
  }
  if( !isset($author->data['lastname']) ) {
    $author->data['lastname'] = $author->name;
  }
    return array(
        'name' => 'author',
        'attributes' => array(
            'primary_contact' => 'true',
        ),
        array(
            'name' => 'firstname',
            'value' => $author->data['firstname'],
        ),
        array(
            'name' => 'lastname',
            'value' => $author->data['lastname'],
        ),
        array(
            'name' => 'email',
            'value' => $author->mail,
        ),
    );
}

function generate_article( $article, $file_absolute_url ){
  $author = generate_author(user_load($article->uid));

  $article = array(
        'name' => 'article',
        array(
            'name' => 'title',
            'attributes' => array(
                'locale' => 'en_US',
            ),
            'value' => $article->dc_title,
        ),
        array(
            'name' => 'abstract',
            'attributes' => array(
                'locale' => 'en_US',
            ),
            'value' => $article->description_en,
        ),
        array(
            'name' => 'abstract',
            'attributes' => array(
                'locale' => 'en_US',
            ),
            'value' => $article->description_sub,
        ),
        array(
            'name' => 'date_published',
            'value' => date('Y-m-d', $article->dc_date),
        ),
        array(
            'name' => 'supplimental_file',
            'attributes' => array(
                'language' => 'en',
                // 'public_id' => $article->jid,
                'public_id' => rand(),
                'type' => 'other',
            ),
            array(
                'name' => 'title',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_title,
            ),
            array(
                'name' => 'creator',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_creator,
            ),
            array(
                'name' => 'publisher',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_publisher,
            ),
            array(
                'name' => 'sponsor',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_contributor,
            ),
            array(
                'name' => 'description',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_description,
            ),
            array(
                'name' => 'type_other',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_type,
            ),
            array(
                'name' => 'subject',
                'attributes' => array(
                    'locale' => 'en_US',
                ),
                'value' => $article->dc_subject,
            ),
            array(
                'name' => 'date_created',
                'value' => date('Y-m-d', $article->dc_date),
            ),
            array(
                'name' => 'file',
                array(
                    'name' => 'href',
                    'attributes' => array(
                        'mime_type' => $article->dc_format,
                        'src' => $file_absolute_url,
                    ),
                ),
            ),
        )
  );
        $article[] = $author;

        return $article;
}

function generate_section( $section, $articles ) {
  $section = array(
    'name' => 'section',
    array(
        'name' => 'title',
        'attributes' => array(
            'locale' => 'en_US'
        ),
        'value' => $section->title
    ),
    array(
        'name' => 'abbrev',
        'attributes' => array(
            'locale' => 'en_US'
        ),
        'value' =>  $section->abbrev
    ),
  );

  foreach ($articles as $article) {
    $section[] = $article;
  }

  return $section;
}

function generate_issue( $issue, $sections ) {
  $issue = array(
    'name' => 'issue',
    'attributes' => array(
      'current' => (string)$issue->current,
      'published' => (string)$issue->published,
      'public_id' => $issue->id,
    ),
    array(
      'name' => 'title',
      'attributes' => array(
          'locale' => 'en_US'
      ),
      'value' => $issue->title,
    ),
    array(
      'name' => 'description',
      'attributes' => array(
          'locale' => 'en_US'
      ),
      'value' => $issue->description,
    ),
    array(
      'name' => 'volumn',
      'value' => $issue->volumn,
    ),
    array(
      'name' => 'number',
      'value' => $issue->number,
    ),
    array(
      'name' => 'year',
      'value' => $issue->year,
    ),
    array(
      'name' => 'date_published',
      'value' =>  date('Y-m-d', $issue->date_published),
    ),
    array(
      'name' => 'access_date',
      'value' => date('Y-m-d', $issue->access_date),
    ),
  );

  foreach ($sections as $section) {
    $issue[] = $section;
  }

  return $issue;
}

function get_journal_section( $section_id ) {
  $section = open_journal_load_section( $section_id );
  // $all_journal = open_journal_load_multiple( $section_id );

  $articles = array();
  for ($i=0; $i < count($section->articles); $i++) {
    $file_last = open_journal_get_file_list($section->articles[$i]->jid, 1);
    $file = file_load($file_last[0]->fid);
    $file_url = file_create_url($file->uri);
    $file_absolute_url = url($file_url, array('absolute' => TRUE));
    $articles[]= generate_article($section->articles[$i], $file_absolute_url);
  }
  return generate_section( $section, $articles );
}

function get_journal_issue( $issue_id ) {
  $issue = open_journal_load_issue($issue_id);

  $sections = array();
  for ($i=0; $i < count($issue->sections); $i++) {
    $sections[] = get_journal_section($issue->sections[$i]);
  }

  return generate_issue( $issue, $sections );
}

function get_journal_all() {
  $issue_list = open_journal_load_all_issue();

  $issues = array(
    'name' => 'issues',
  );
  for ($i=0; $i < count($issue_list->issues); $i++) {
    $issues[] = get_journal_issue($issue_list->issues[$i]);
  }
  dpm($issues);

  return $issues;
}

function open_journal_ojs_export() {
  drupal_add_http_header('Content-Type: text/xml; charset=utf-8');
  global $user;

  $issues = get_journal_all();

  $data = $issues;
  $doc = new DOMDocument();
  $child = generate_xml_element( $doc, $data );
  if ( $child )
      $doc->appendChild( $child );
  $doc->formatOutput = true; // Add whitespace to make easier to read XML
  $xml = $doc->saveXML();

  // dpm('---------------------');
  // dpm(user_load(1));
  // dpm($issues);
  // dpm($xml);

  print $xml;
}