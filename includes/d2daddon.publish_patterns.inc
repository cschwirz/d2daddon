<?php

/**
 * @file
 * Push public patterns to friend instances.
 */
function d2daddon_publish_patterns() {
  $build['main'] = array(
    '#title' => t('Settings'),
    '#type' => 'fieldset',
  );
  $build['main'][] = drupal_get_form('d2daddon_form_publish_settings');
  $build['publish'] = array(
    '#title' => t('Publish patterns'),
    '#type' => 'fieldset',
  );
  $build['publish'][] = drupal_get_form('d2daddon_form_publish_patterns');
  return $build;
}
function d2daddon_form_publish_settings() {
  $form = array();
  $groups = drupaltodrupal_get_groups();
  $options = array();
  $default = variable_get('d2daddon_publish_groups', array());
  foreach ($groups as $group) {
    $options[$group->id] = $group->name;
  }
  $form['auto'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('d2daddon_auto_publish', FALSE),
    '#title' => t('Automatically publish patterns'),
    '#description' => t('If checked, patterns are automatically published every time Drupal\'s cron job is run.'),
  );
  $form['group'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Groups to publish patterns to'),
    '#default_value' => $default,
    '#options' => $options,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save'),
  );
  return $form;
}
function d2daddon_form_publish_settings_submit($form, &$form_state) {
  $groups = drupaltodrupal_get_groups();
  $default = array();
  foreach ($groups as $group) {
    if ($form_state['values']['group'][$group->id] == $group->id) {
      $default[] = $group->id;
    }
  }
  variable_set('d2daddon_publish_groups', $default);
  variable_set('d2daddon_auto_publish', $form_state['values']['auto'] ? TRUE : FALSE);
}
function d2daddon_form_publish_patterns() {
  $form = array();
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Push patterns'),
  );
  return $form;
}
function d2daddon_form_publish_patterns_submit($form, &$form_state) {
  d2daddon_publish_public_patterns();
}
function d2daddon_publish_public_patterns($verbose=TRUE) {
  $patterns = patterns_get_patterns(TRUE, FALSE);
  $patterns = $patterns[PATTERNS_STATUS_OK];
  $clone_patterns = array();
  $n_clone_patterns = 0;
  foreach ($patterns as $pid => $pattern) {
    if (!$pattern->public) {
      continue;
    }
    $name = $pattern->name;
    $title = $pattern->title;
    $content = file_get_contents($pattern->file);
    $format = 'yaml'; // TODO
    $valid_extensions = array('xml', 'yaml'); // TODO allow different extensions?
    foreach ($valid_extensions as $extension) {
      if (strrpos($pattern->file, $extension) === strlen($pattern->file) - strlen($extension)) {
        $format = $extension;
      }
    }
    $version = ''; // TODO: NULL as default?
    if (array_key_exists('version', $pattern->info)) {
      $version = $pattern->info['version'];
    }
    $imploded_pattern = drupaltodrupal_implode(
      array(
        'name' => $name,
        'title' => $title,
        'version' => $version,
        'format' => $format,
        'content' => $content,
      )
    );
    if ($imploded_pattern !== FALSE) {
      $clone_patterns[] = $imploded_pattern;
      $n_clone_patterns++;
    }
  }
  $imploded_patterns = drupaltodrupal_implode($clone_patterns);
  if ($n_clone_patterns == 0) {
    if ($verbose) {
      drupal_set_message(t('No patterns to be published selected.'));
    }
  }
  elseif ($imploded_patterns !== FALSE) {
    $groups = variable_get('d2daddon_publish_groups', array());
    $friends = drupaltodrupal_get_friends_in_group($groups);
    if (empty($groups)) {
      if ($verbose) {
        drupal_set_message(t('No groups selected for publishing.'), 'warning');
      }
    }
    else {
      if (empty($friends)) {
        if ($verbose) {
          drupal_set_message(t('Groups contain no friend instances to which the patterns should be published.'), 'warning');
        }
      }
      else {
        foreach ($friends as $friend) {
          $error_string = '';
          $res = drupaltodrupal_call_secure_rpc($friend, 'd2daddon_push_patterns', array('patterns' => $imploded_patterns), $error_string);
          if ($res === FALSE) {
            $description = check_plain($error_string);
          }
          else {
            if ($res === '') {
              $description = t('pushed patterns');
            }
            else {
              $description = t('error:') . ' ' . check_plain($res);
            }
          }
          if ($verbose) {
            drupal_set_message($friend['url'] . ' - ' . $description);
          }
        }
      }
    }
  }
  else {
    drupal_set_message(t('Error imploding patterns'), 'error');
  }
}
function d2daddon_push_patterns($arguments, $rpc_info) {
  $imploded_patterns = $arguments['patterns'];
  $exploded_patterns = drupaltodrupal_explode($imploded_patterns);
  if ($exploded_patterns === FALSE) {
    return 'explosion failed';
  }
  foreach ($exploded_patterns as $imploded_pattern) {
    $exploded_pattern = drupaltodrupal_explode($imploded_pattern);
    if ($exploded_pattern !== FALSE) {
      $check = array(
        'name' => 'is_string',
        'title' => 'is_string',
        'version' => 'is_string',
        'format' => 'is_string',
        'content' => 'is_string',
      );
      if (drupaltodrupal_check_array($exploded_pattern, $check)) {
        patterns_io_save_pattern($exploded_pattern['content'], $rpc_info['id'] . '_' . $exploded_pattern['name'], $exploded_pattern['format']);
      }
    }
  }
  return '';
}
