<?php
/**
 * @file
 * commerce_order_tracking_number.
 */

// Store admin page for creating an order tracking number.
// /?q=admin/commerce/orders/tracking page.
function commerce_order_tracking_number_admin_page() {
  // Tracking my orders
  global $user;
  $build = array();
  
  $query = db_select('commerce_order', 'c');
  $query
    ->fields('c')
    ->orderBy('c.order_id', 'DESC');
  $query
    ->leftjoin('commerce_order_tracking_number', 't', 'c.order_id = t.order_id');
  $query->addField('t', 'uid', 't_uid');
  $query->addField('t', 'order_id', 't_order_id');
  $query->addField('t', 'tracking_id', 't_tracking_id');
  $query->addField('t', 'shipper_name', 't_shipper_name');
  $result = $query->execute();
 

  foreach ($result as $row) {
    if (($row->status != "pending") && ($row->status != "completed")) {
      continue;
    }
    $offer = '';
     
    $color = drupal_substr(MD5($row->order_id), 0, 6);
    $this_order = commerce_order_load($row->order_id);
    $this_user = user_load($row->uid);
    if (isset($this_order->commerce_customer_shipping['und']['0']['profile_id'])) {
      $this_user_profile = commerce_customer_profile_load($this_order->commerce_customer_shipping['und']['0']['profile_id']);
    }
    else {
      // No user.
    }
    
    
     
    $shipped = 'ADD TRACKING NUMBER';
    $shiplink =  l($shipped, 'admin/commerce/orders/' . $this_order->order_id . '/track');
    
    
    if ($row->t_tracking_id != '') {
    
        $shipped = $row->t_tracking_id;
        $shiplink =  l($shipped, 'admin/commerce/orders/' . $this_order->order_id);
      
    }

    $rows[] = array(
          '<div style="width:20px;height:20px;float:left;margin-right:10px;background:#' . $color . '"></div>' .  
          l($this_order->order_id, 'admin/commerce/orders/' . $this_order->order_id),
          l($this_order->mail, 'user/' . $this_order->uid),
          $this_user_profile->commerce_customer_address['und']['0']['locality'] . ', ' .
          $this_user_profile->commerce_customer_address['und']['0']['administrative_area'] . ' ' .
          $this_user_profile->commerce_customer_address['und']['0']['postal_code'],
          format_date($row->changed),
          $row->t_shipper_name ,
          $shiplink
    );
    
  
  }
  if (empty($rows)) {
    $rows[] = array(
          '<div style="width:20px;height:20px;float:left;margin-right:10px;background:#666666"></div>',
          'No orders have tracking numbers, or you have no active orders.',
          '',
          '',
          '',
          '',
    );
  }
    $header = array('Order ID', 'Customer', 'Ship To', 'Updated', 'Shipped With', 'Shipped');
    // Format output.
    $output = "<h4>To select a person that will receive your hand-me-down items, click in the offer column to make an offer.</h4>";
    $output .= theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');

  return $output;
}


// User page to see orders.
// commerce_order_tracking_number_page function
//
//  user/%userid/order_tracking
//
function commerce_order_tracking_number_user_page() {
  // Tracking my orders
  global $user;
  $build = array();

	$ups_link = "http://wwwapps.ups.com/WebTracking/track?track=yes&trackNums=";
	$dhl_link = "http://track.dhl-usa.com/TrackByNbr.asp?ShipmentNumber=";
	$usps_link = "https://tools.usps.com/go/TrackConfirmAction_input?qtc_tLabels1=";
	$fedex_link = "http://www.fedex.com/Tracking?action=track&tracknumbers=";

  $query = db_select('commerce_order', 'c');
  $query
    ->fields('c')
    ->condition('c.uid', $user->uid, '=');
  $query
    ->leftjoin('commerce_order_tracking_number', 't', 'c.order_id = t.order_id');
  $query->addField('t', 'uid', 't_uid');
  $query->addField('t', 'order_id', 't_order_id');
  $query->addField('t', 'tracking_id', 't_tracking_id');
  $query->addField('t', 'ship_date', 't_ship_date');
  $query->addField('t', 'shipper_name', 't_shipper_name');
  $result = $query->execute();
 

  foreach ($result as $row) {
    if (($row->status != "pending") && ($row->status != "completed")) {
      continue;
    }

    $offer = '';
     
    $color = drupal_substr(MD5($row->order_id), 0, 6);
    $this_order = commerce_order_load($row->order_id);
    $this_user = user_load($row->uid);
    $this_user_profile =  commerce_customer_profile_load($this_order->commerce_customer_shipping['und']['0']['profile_id']);
    $shippeddate = "";
    if (isset($row->t_ship_date)) {
      $shippeddate = date("Y-m-d", $row->t_ship_date);
      
    }
     
    $shipped = 'Check Status';
    $shiplink =  l($shipped, 'user/' . $user->uid . '/orders');
    
    
    if ($row->t_tracking_id != '') {
    
      $shipped = $row->t_tracking_id;
      $shiplink =  l($shipped, 'http://wwwapps.ups.com/WebTracking/track?track=yes&trackNums=' . $row->t_tracking_id);
      
    }

    $rows[] = array(
          '<div style="width:20px;height:20px;float:left;margin-right:10px;background:#' . $color . '"></div>' .  
          l($this_order->order_id, 'user/' . $user->uid . '/orders/' . $this_order->order_id),
          l($this_order->mail, 'user/' . $this_order->uid),
          $this_user_profile->commerce_customer_address['und']['0']['locality'] . ', ' .
          $this_user_profile->commerce_customer_address['und']['0']['administrative_area'] . ' ' .
          $this_user_profile->commerce_customer_address['und']['0']['postal_code'],
          $shippeddate,
          $row->t_shipper_name,
          $shiplink,
    );
    
  
  }
  if (empty($rows)) {
    $rows[] = array(
          '<div style="width:20px;height:20px;float:left;margin-right:10px;background:#666666"></div>',
          'No orders have tracking numbers, or you have no active orders.',
          '',
          '',
          '',
          '',
    );
  }
    $header = array('Order ID', 'Customer', 'Ship To', 'Ship Date', 'Shipped With', 'Shipped');
    // Format output.
    $output = "<h4>Please read our <a href=\"/FAQ\">FAQ</a> before contacting us about a shippment.</h4>";
    $output .= theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');

  return $output;
}





//  Shipping Form to add tracking number
//
//
//  admin/commerce/orders/%order_id/ship
//
function commerce_order_tracking_form($form, &$form_state, $order) {
  global $base_url;

  if (!isset($order->order_id)) {
    drupal_set_message('Order is not set.');
    return;
  }

  $this_order = $order;
  $order_id = $order->order_id;
  $uid = $order->uid;
  $tracking_id = "";
  $user = user_load($uid);
  $today = strtotime('today');
  $datestr = date("Y-m-d", $today);
  $shipperlist = _commerce_order_tracking_number_shipper_list();

  $query = db_select('commerce_order_tracking_number', 't');
  $query
    ->fields('t')
    ->condition('t.order_id', $order_id, '=');
  $result = $query->execute();


  foreach ($result as $row) {
    $tracking_id = $row->tracking_id;
    $shippedwith = $row->shipper_name;
    $ship_date = $row->ship_date;
  }

  $shipinfo = "User " . l($user->name, "user/$uid") . " placed order number $order_id";
  if ($tracking_id > 0) {
    $shipinfo .= " shipped with $shippedwith tracking number $tracking_id.";
  }

  $form['tracking']['content'] = array(
      '#title' => t('Tracking'),
      '#type' => 'markup',
    
      '#markup' =>  '<div><h3>' . $shipinfo . '</h3></div>',
  );
  $form['tracking']['order_id'] = array(
    '#type' => 'value',
    '#title' => t('Order Number'),
    '#value' => $order_id,
  );
  $form['tracking']['tracking_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking Number'),
    '#description' => t('Enter 0 to delete the tracking item.'),
    '#required' => TRUE,
    '#size' => 30,
    '#maxlength' => 128,
    '#default_value' => $tracking_id,
  );
  $options =  array('UPS' => 'UPS');
  $form['tracking']['shippedwith'] = array(
    '#type' => 'select',
    '#title' => t('Shipped With'),
    '#options' => $shipperlist,
    '#default_value' => variable_get('commerce_order_tracking_number_shipper', $shipperlist['UPS']),
    '#description' => t('Shipped with this carrier.'),
  );
  $form['tracking']['ship_date'] = array(
    '#type' => 'textfield',
    '#title' => t('Ship Date'),
    '#description' => t('Date the order shipped.'),
    '#required' => TRUE,
    '#default_value' => $datestr,
  );

   $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('commerce_order_tracking_form_submit'),
  );


  return $form;
}



// Submit function.
function commerce_order_tracking_form_submit($form, &$form_state) {
  if (!user_access('administer content')) {
    return;
  }

  $logmessage = "";

  $order_id = $form_state['values']['order_id'];
  $tracking_id = $form_state['values']['tracking_id'];
  $shippedwith = $form_state['values']['shippedwith'];
  $ship_date = $form_state['values']['ship_date'];

  $this_order = commerce_order_load($order_id);
  $uid = $this_order->uid;
  $status_name = $this_order->status;

  if ($order_id == '') {
    drupal_set_message('No order id given to form submit.');
    return;
  }

  if ($tracking_id == '0') {
    $logmessage = t('Remove tracking number if any.');
    db_delete('commerce_order_tracking_number')
      ->condition('order_id', $order_id)
      ->execute();

    if ($this_order->status == 'completed') {
      $status_name = 'pending';
    }
  }
  else {
    $logmessage = "Added $shippedwith tracking number $tracking_id to order number $order_id.";
    // Delete any existing tracking numbers.  This will need
    // to be removed to allow multiple packages.
    db_delete('commerce_order_tracking_number')
      ->condition('order_id', $order_id)
      ->execute();

    // Add the entry to the DB.
    db_insert('commerce_order_tracking_number')
      ->fields(array(
        'order_id' => $order_id,
        'uid' => $uid,
        'tracking_id' => $tracking_id,
        'ship_date' => strtotime($ship_date),
        'shipper_name' => $shippedwith,
      ))
      ->execute();
    if ($this_order->status == 'pending') {
      $status_name = 'completed';
    }

  }

  commerce_order_status_update($this_order, $status_name, FALSE, TRUE, $logmessage);
  // commerce_order_tracking_number_mail($this_order)

  drupal_goto('admin/commerce/orders/tracking');

  return;

}