<?php

/**
 * @file
 * Receive, send, and show system information of friend instances.
 */
function d2daddon_show_info() {
  $build['main'] = array(
    '#title' => t('System information for friend instances'),
    '#type' => 'fieldset',
  );
  $friends = drupaltodrupal_get_friends();
  $all_rows = array();
  foreach ($friends as $friend) {
    $error_string = '';
    $all_rows[] = array(
      array(
        'data' => check_plain($friend['url']),
        'style' => 'font-weight: bold',
      ),
      '',
    );
    $res = drupaltodrupal_call_secure_rpc($friend, 'd2daddon_info', array(), $error_string);
    $rows = array();
    $description = t('no information received');
    if ($res === FALSE) {
      $description = check_plain($error_string);
    }
    else {
      $exploded_res = drupaltodrupal_explode($res);
      if ($exploded_res === FALSE) {
        $description = t('erroneous return:') . ' ' . check_plain($res);
      }
      else {
        foreach ($exploded_res as $key => $value) {
          $rows[] = array(check_plain($key), check_plain($value));
        }
      }
    }
    if (empty($rows)) {
      $all_rows[] = array(
        array(
          'data' => '&nbsp;&nbsp;&nbsp;&nbsp;' . $description,
          'style' => 'font-style: italic',
        ),
        '',
      );
    }
    else {
      foreach ($rows as $row) {
        $all_rows[] = array('&nbsp;&nbsp;&nbsp;&nbsp;' . $row[0], $row[1]);
      }
    }
  }
  $build['main'][] = array(
    '#theme' => 'table',
    '#header' => array(t('Type'), t('Value')),
    '#rows' => $all_rows,
    '#empty' => t('No friends found in the database'),
  );
  return $build;
}

function d2daddon_srpc_info($arguments, $rpc_info) {
  $friends = drupaltodrupal_get_friends();
  $n_friends = sizeof($friends);
  $software = $_SERVER['SERVER_SOFTWARE'];
  $phpversion = phpversion();
  $class = 'DatabaseTasks_' . Database::getConnection()->driver();
  $tasks = new $class();
  $dbname = '';
  $dbversion = '';
  $dbname = $tasks->name();
  $dbversion = Database::getConnection()->version();
  $return_array = array(
    'time' => date('l jS \of F Y h:i:s A'),
    'drupal version' => VERSION,
    'web server' => $software,
    'php version' => $phpversion,
    'database' => $dbname . $dbversion,
    'number of friends' => $n_friends,
    //'installed modules' => module_list(),
  );
  foreach ($return_array as &$value) {
    $value = strval($value);
  }
  $imploded_return_array = drupaltodrupal_implode($return_array);
  if ($imploded_return_array === FALSE) {
    return '';
  }
  return $imploded_return_array;
}
