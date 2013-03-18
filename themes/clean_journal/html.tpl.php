<?php

/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup themeable
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
  <div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
  

  <div id="header-profile" class="<?php print $header_class; ?>">

    <?php if ($logged_in): ?>
    <div id="header-content">
      <?php if(!empty($open_journal_logo1)):?>
      <div id="open-journal-small-logo">
        <?php print l($open_journal_logo1, '', array('html' => TRUE));?>
      </div>
      <div class="user-manage">
        <?php print l(t('View My article'), OPEN_JOURNAL_PREFIX_PATH, array('attributes' => array('class' => array('view-all-journal')))); ?>
        <div class="user-dropdown">
          <?php print $user_info['display_picture_small']; ?>
          <span class="dropdown-link"><?php print $user_info['display_name']; ?></span>
          <ul class="dropdown active-dropdown">
            <?php if (user_access('export issue to ojs xml format')): ?>
            <li><?php print l(t('OJS export'), OPEN_JOURNAL_PREFIX_PATH.'/ojs/export'); ?></li>  
            <?php endif ?>
            <?php if (user_access('create update delete issue')): ?>
            <li><?php print l(t('All issue'), 'issue'); ?></li>  
            <?php endif ?>  
            <li><?php print l(t('Add new article'), OPEN_JOURNAL_PREFIX_PATH.'/add'); ?></li>
            <li><?php print l(t('Account settings'), OPEN_JOURNAL_PREFIX_PATH.'/account/setting'); ?></li>
            <li><?php print l(t('Logout'), 'user/logout'); ?></li>
          </ul>
        </div>
      </div>
    </div>
    <?php endif;?>
    <?php else: ?>
  	<p>Welcome to E-Journal</p>
    <?php endif;?>

  </div>

  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
  
  <div id="footer">
  	<p>E-Journal © 2013 Health Systems Research Institute (HSRI)
    <?php print l(theme('image', array('path' => drupal_get_path('module', 'open_journal').'/images/rss.png') ), OPEN_JOURNAL_PREFIX_PATH.'/feed/issue', array('html' => TRUE, 'attributes' => array('class' => 'feed', 'title' => 'Issue rss feed')));?>
    <?php //print l(theme('image', array('path' => drupal_get_path('module', 'open_journal').'/images/rss.png') ), OPEN_JOURNAL_PREFIX_PATH.'/feed/article', array('html' => TRUE, 'attributes' => array('class' => 'feed', 'title' => 'Article rss feed')));?>
    </p>
  </div>
  
</body>
</html>
