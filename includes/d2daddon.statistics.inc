<?php

/**
 * @file
 * Receive, send, and show system information of friend instances.
 */
function _d2daddon_compare_rating_asc($a, $b) {
  return $a['3'] < $b['3']; // TODO: wrong
}
function _d2daddon_compare_count_desc($a, $b) {
  return $a['1'] > $b['1']; // TODO: wrong
}
function d2daddon_check_convert_float(&$x) {
  if ((float)$x) {
    $x = floatval($x);
    return TRUE;
  }
  return FALSE;
}
function d2daddon_statistics_query($community, $query) {
  $error_string = '';
  if ($community['id'] === NULL) {
    $res = d2daddon_statistics_get_query_results(array('query' => $query), NULL);
  }
  else {
    $res = drupaltodrupal_call_secure_rpc($community, 'd2daddon_get_query_results',
                                            array(
                                              'query' => $query
                                            ),
                                            $error_string
                                          );
  }
  if ($res === FALSE) {
    return t('Error retrieving information.');
  }
  $exploded_res = drupaltodrupal_explode($res);
  if ($exploded_res === FALSE) {
    return t('Error retrieving information.');
  }
  $result = array();
  foreach ($exploded_res as $imploded_row) {
    $exploded_row = drupaltodrupal_explode($imploded_row);
    if ($exploded_row !== FALSE) {
      $result[] = $exploded_row;
    }
  }
  return $result;
}
function d2daddon_statistics_best_paper($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT author, title, rating, votes FROM {d2daddon_statistics_papers} ORDER BY rating DESC LIMIT 1');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('author' => 'is_string', 'title' => 'is_string', 'rating' => 'd2daddon_check_convert_float', 'votes' => 'd2daddon_check_convert_float'))) {
          $row = array($community['name'], $line['author'], $line['title'], $line['rating'], $line['votes']);
          $rows[] = $row;
        }
      }
    }
    else {
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best papers</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_best_news($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT author, title, rating, votes, created FROM {d2daddon_statistics_news} ORDER BY rating DESC LIMIT 1');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('author' => 'is_string', 'title' => 'is_string', 'rating' => 'd2daddon_check_convert_float', 'votes' => 'd2daddon_check_convert_float', 'created' => 'drupaltodrupal_check_convert_int'))) {
          $row = array($community['name'], $line['author'], $line['title'], $line['rating'], $line['votes'], date('Y-m-d', $line['created']));
          $rows[] = $row;
        }
      }
    }
    else {
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best rated news</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_latest_news($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT author, title, rating, votes, created FROM {d2daddon_statistics_news} ORDER BY created DESC LIMIT 1');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('author' => 'is_string', 'title' => 'is_string', 'rating' => 'd2daddon_check_convert_float', 'votes' => 'd2daddon_check_convert_float', 'created' => 'drupaltodrupal_check_convert_int'))) {
          $row = array($community['name'], $line['author'], $line['title'], $line['rating'], $line['votes'], date('Y-m-d', $line['created']));
          $rows[] = $row;
        }
      }
    }
    else {
      //$rows[] = array($community['name'], $result);
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Latest news</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_userstat($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT COUNT(id) as count, AVG(posts) as posts, AVG(votes) as votes FROM {d2daddon_statistics_user}');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('count' => 'drupaltodrupal_check_convert_int', 'posts' => 'd2daddon_check_convert_float', 'votes' => 'd2daddon_check_convert_float'))) {
          $row = array($community['name'], $line['count'], $line['posts'], $line['votes']);
          $rows[] = $row;
        }
      }
    }
    else {
      //$rows[] = array($community['name'], $result);
    }
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Number of users', 'Average post count', 'Average vote count'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>User statistics</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_all_news($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT author, title, rating, votes, created FROM {d2daddon_statistics_news} ORDER BY created DESC');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('author' => 'is_string', 'title' => 'is_string', 'rating' => 'd2daddon_check_convert_float', 'votes' => 'drupaltodrupal_check_convert_int', 'created' => 'drupaltodrupal_check_convert_int'))) {
          $row = array($community['name'], $line['author'], $line['title'], $line['rating'], $line['votes'], date('Y-m-d', $line['created']));
          $rows[] = $row;
        }
      }
    }
    else {
      //$rows[] = array($community['name'], $result);
    }
  }
  usort($rows, '_d2daddon_compare_rating_asc');
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Author', 'Title', 'Rating', 'Votes', 'Date'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Best news orderer by rating</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_community_activity($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $result = d2daddon_statistics_query($community, 'SELECT COUNT(*) as count FROM {d2daddon_statistics_papers}');
    if (is_array($result)) {
      foreach ($result as $line) {
        if (drupaltodrupal_check_array($line, array('count' => 'drupaltodrupal_check_convert_int'))) {
          $row = array($community['name'], $line['count']);
          $rows[] = $row;
        }
      }
    }
    else {
      //$rows[] = array($community['name'], $result);
    }
  }
  usort($rows, '_d2daddon_compare_count_desc');
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', 'Number of papers'),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Community activity</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_statistics_list_communities($communities) {
  $rows = array();
  foreach ($communities as $community) {
    $link = $community['id'] === NULL ? 'this community' : l($community['url'], $community['url']);
    $rows[] = array($community['name'], $link);
  }
  $build['node_table'] = array(
    '#theme' => 'table',
    '#header' => array('Community', ''),
    '#rows' => $rows,
    '#empty' => t('No information available.'),
    '#prefix' => '<div><h2>Communities</h2>',
    '#suffix' => '</div>',
  );
  return $build;
}
function d2daddon_show_statistics() {
  $build['main'] = array(
    '#title' => t('Collected statistics for remote instances'),
    '#type' => 'fieldset',
  );
  $communities = array();
  $communities[] = array(
    'name' => variable_get('d2daddon_statistics_community', D2DADDON_STATISTICS_COMMUNITY),
    'id' => NULL,
  );
  $friends = drupaltodrupal_get_friends();
  foreach ($friends as $friend) {
    $error_string = '';
    $res = drupaltodrupal_call_secure_rpc($friend, 'd2daddon_get_community_name', array(), $error_string);
    if ($res === FALSE) {
    }
    else {
      $community = $friend;
      $community['name'] = $res;
      $communities[] = $community;
    }
  }
  $build['main'][] = d2daddon_statistics_list_communities($communities);
  $build['main'][] = d2daddon_statistics_best_paper($communities);
  $build['main'][] = d2daddon_statistics_best_news($communities);
  $build['main'][] = d2daddon_statistics_latest_news($communities);
  $build['main'][] = d2daddon_statistics_userstat($communities);
  $build['main'][] = d2daddon_statistics_all_news($communities);
  $build['main'][] = d2daddon_statistics_community_activity($communities);
  return $build;
}
function d2daddon_statistics_get_news_sets() {
  return array('a', 'b', 'c', 'd');
}
function d2daddon_configure_statistics_form() {
  $form = array();
  $form['community'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of the community'),
    '#default_value' => variable_get('d2daddon_statistics_community', D2DADDON_STATISTICS_COMMUNITY),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['author'] = array(
    '#type' => 'textfield',
    '#title' => t('Author'),
    '#description' => t('Name of the person that will be shown as author of the news.'),
    '#default_value' => variable_get('d2daddon_statistics_author', D2DADDON_STATISTICS_AUTHOR),
    '#size' => 64,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['user_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of users'),
    '#description' => t('Number of users in the community.'),
    '#default_value' => variable_get('d2daddon_statistics_user_count', D2DADDON_STATISTICS_USER_COUNT),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  $form['news_set'] = array(
    '#type' => 'textfield',
    '#title' => t('News set'),
    '#description' => t('String identifying the news set to be used.'),
    '#default_value' => variable_get('d2daddon_statistics_news_set', D2DADDON_STATISTICS_NEWS_SET),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save & Create'),
  );
  return $form;
}
function d2daddon_configure_statistics_form_validate($form, &$form_state) {
  $user_count = $form_state['values']['user_count'];
  if (!drupaltodrupal_check_convert_int($user_count)) {
    form_set_error('utc_offset', t('Number of users must be an integer.'));
  }
  elseif ($user_count < 0 || $user_count > 1000) {
    form_set_error('utc_offset', t('User count must be between @min and @max.', array('@min' => 1, '@max' => 1000)));
  }
  $news_set = $form_state['values']['news_set'];
  if (!in_array($news_set, d2daddon_statistics_get_news_sets())) {
    $news_sets = d2daddon_statistics_get_news_sets();
    form_set_error('news_set', t('Given string does not match a set of news. Valid strings are:') . ' ' . implode($news_sets, ','));
  }
}
function d2daddon_configure_statistics_form_submit($form, &$form_state) {
  variable_set('d2daddon_statistics_community', $form_state['values']['community']);
  variable_set('d2daddon_statistics_author', $form_state['values']['author']);
  $user_count = $form_state['values']['user_count'];
  drupaltodrupal_check_convert_int($user_count);
  variable_set('d2daddon_statistics_user_count', $user_count);
  variable_set('d2daddon_statistics_news_set', $form_state['values']['news_set']);
  drupal_set_message(t('Settings have been saved.'));
  d2daddon_statistics_create_users();
  d2daddon_statistics_add_papers();
  d2daddon_statistics_add_news();
  drupal_set_message(t('(Dummy) data has been inserted into database.'));
}
function d2daddon_configure_statistics() {
  $build['main'] = array(
    '#title' => t('Provided Statistics - Settings'),
    '#type' => 'fieldset',
  );
  $build['main'][] = drupal_get_form('d2daddon_configure_statistics_form');
  return $build;
}
function d2daddon_statistics_get_community_name($arguments, $rpc_info) {
  return variable_get('d2daddon_statistics_community', D2DADDON_STATISTICS_COMMUNITY);
}
     
function d2daddon_statistics_get_query_results($arguments, $rpc_info) {
  $query = $arguments['query'];
  $result = db_query('SELECT id, query FROM {d2daddon_statistics_query_whitelist}');
  $flag = FALSE;
  foreach ($result as $record) {
    // if ($query === $record->query)
    if (preg_match($record->query, $query)) {
      $flag = TRUE;
    }
  }
  if (!$flag) {
    return '';
  }
  $tmp_res = db_query($query);
  watchdog('d2daddon', $query);
  $result = array();
  foreach ($tmp_res as $record) {
    $rec = array();
    foreach ($record as $key => $value) {
      $rec[$key] = $value;
    }
    $imploded_record = drupaltodrupal_implode($rec);
    if ($imploded_record !== FALSE) {
      $result[] = $imploded_record;
    }
  }
  $imploded_result = drupaltodrupal_implode($result);
  if ($imploded_result === FALSE) {
    return '';
  }
  return $imploded_result;
}

