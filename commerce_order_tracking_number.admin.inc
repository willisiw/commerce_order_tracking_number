<?php
// $Id$

/**
 * @file
 * Admin page callback for the scaffolding module.
 */

/**
 * Builds and returns the scaffolding settings form.
 */
function commerce_order_tracking_number_admin_settings() {
  $helptxt = '';
  $links = commerce_order_tracking_number_menu();  
  foreach ($links as $link => $v) {
    $helptxt .= '<p>' . l($v['title'] . ' - ' . $v['description'],$link) . '</p>';
  }

  $form['help'] = array(
      '#type' => 'markup',
      '#preface' => '<div>',
      '#markup' => $helptxt,
      '#suffix' => '</div>',
  );


  $shipperlist = _commerce_order_tracking_number_shipper_list();
  $form['commerce_order_tracking_number_shipper'] = array(
    '#type' => 'select',
    '#size' => '1',
    '#title' => t('Default Shipper'),
    '#options' => $shipperlist,
    '#default_value' => variable_get('commerce_order_tracking_number_shipper', $shipperlist[0]),
    '#description' => t('Select the default Shipper for order tracking.  This only enables the addition of a text label and URL link for package tracking purposes only.'),
  );

  return system_settings_form($form);
}