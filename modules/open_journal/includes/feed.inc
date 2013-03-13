<?php

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


function open_journal_feed_generator($data) {
    dpm("GOGO");
    dpm(open_journal_load_multiple());
    return "GENERATOR";
}

?>