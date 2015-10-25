<?php

ini_set('display_errors', 'On');
error_reporting(E_ALL);

$topdir=realpath(".");
include('smarty/Smarty.class.php');
$smarty = new Smarty;
$smarty->setTemplateDir($topdir.'/smarty/templates');
$smarty->setCompileDir($topdir.'/smarty/templates_c');
$smarty->setCacheDir($topdir.'/smarty/cache');
$smarty->setConfigDir($topdir.'/smarty/configs');

require 'config.php';
require 'HotelGatewayConnection.php';

try{
	$hotel_gateway_connection = new HotelGatewayConnection($HOTELGW_ADDRESS, $HOTELGW_PORT);
	$response = $hotel_gateway_connection->request('g');

	$guest_list = explode("\n", $response);
	unset($guest_list[count($guest_list)-1]);//last one is empty

	$smarty->assign('guest_list', $guest_list);
}
catch(Exception $e){
	$smarty->assign('error', $e->getMessage());
}

$smarty->display('tpl/participants.html');