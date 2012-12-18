-- SUMMARY --

Add a tracking number that displays a user link to order online tracking.

Configuration is in Store > Configuration > Order Tracking 
or at /?q=admin/commerce/config/commerce_order_tracking

The module assumes that orders ready to be shipped have a status of "pending".
Navigate to Store > Orders > Track Orders or /?q=admin/commerce/orders/tracking

To add a package tracking number to an order, open an order from the 
Store > Orders page, or the Store > Orders > Track Orders page, or click
the ADD TRACKING NUMBER link on the Track Orders page.

* Enter a tracking number.

* Select a carrier.  This will determine the URL used to link the user to live 
tracking information.

* Optionally enter the package ship date.  This is informational only for the 
user.

* Click Save.  Upon clicking Save, "pending" orders status will be set to 
"completed".

 

For a full description of the module, visit the project page:
  http://drupal.org/sandbox/willisiw/xxxxxx

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/node/add/project-issue/xxxxxxxx


-- REQUIREMENTS --

* Drupal Commerce

-- INSTALLATION --

* Install module using the zip file.  For further info visit 
  http://drupal.org/documentation/install/modules-themes/modules-7

-- CONFIGURATION --

* After enabling the module, navigate to the configuration page.

* Set defaults and save. 


-- TROUBLESHOOTING --


-- FAQ --


-- CONTACT --

Current maintainers:
* Ian Willis (willisiw) - willisiw@754180.no-reply.drupal.org
