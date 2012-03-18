<?php

/**
 * @file
 * Run code on remote instance.
 */
function d2daddon_remote_control_form() {
  $form = array();
  $friends = drupaltodrupal_get_friends();
  if (empty($friends)) {
    drupal_set_message(t('No friends found in database'));
    return $form;
  }
  $options = array();
  $last_id = variable_get('d2daddon_remote_control_last_id');
  $proposed_id = null;
  foreach ($friends as $friend) {
    $options[$friend['id']] = $friend['url'];
    if (is_null($proposed_id) || $friend['id'] == $last_id) {
      $proposed_id = $friend['id'];
    }
  }
  $form['friend'] = array(
    '#type' => 'radios',
    '#title' => t('Instance to run code on'),
    '#default_value' => $proposed_id, 
    '#options' => $options,
  );
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP Code to run'),
    '#description' => t('The provided code is evaluated using PHP\'s eval-function.'),
    '#rows' => 10,
    '#cols' => 60,
    '#default_value' => variable_get('d2daddon_remote_control_code', ''),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Run code'),
  );
  return $form;
}
function d2daddon_remote_control_form_submit($form, &$form_state) {
  variable_set('d2daddon_remote_control_code', $form_state['values']['code']);
  $friend_id = $form_state['values']['friend'];
  variable_set('d2daddon_remote_control_last_id', $friend_id);
  $friends = drupaltodrupal_get_friends();
  foreach ($friends as $friend) {
    if ($friend_id === $friend['id']) {
      $res = drupaltodrupal_call_secure_rpc($friend, 'd2daddon_remote_control', array('code' => $form_state['values']['code']), $error_string);
      if ($res === FALSE) {
        drupal_set_message(t('Error: @message', array('@message' => $error_string)));
      }
      else {
        drupal_set_message(t('Method returned \'@return\'.', array('@return' => $res)));
      }
      return;
    }
  }
  drupal_set_message('No friend selected.', 'warning');
}
function d2daddon_remote_control() {
  $build['main'] = array(
    '#title' => t('Run code on remote instance'),
    '#type' => 'fieldset',
  );
  $build['main'][] = drupal_get_form('d2daddon_remote_control_form');
  return $build;
}
function d2daddon_srpc_remote_control($arguments, $rpc_info) {
  return eval($arguments['code']);
}
