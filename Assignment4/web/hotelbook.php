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

if(isset($_POST['name'], $_POST['type'])){
	if(empty($_POST['name'])){
		$smarty->assign('error', 'No name supplied');
	}
	elseif(empty($_POST['type'])){
		$smarty->assign('error', 'No room type supplied');
	}
	elseif(intval($_POST['type']) != $_POST['type']){
		$smarty->assign('error', 'Invalid room type');
	}
	else{
		require 'HotelGatewayConnection.php';
		require 'config.php';
		
		try{
			$hotel_gateway_connection = new HotelGatewayConnection($HOTELGW_ADDRESS, $HOTELGW_PORT);
			$response = $hotel_gateway_connection->request('b ' . $_POST['type'] . ' ' . $_POST['name']);

			header('Location: hotelbook.php?response=' . urlencode($response));
			die();
		}
		catch(Exception $e){
			$smarty->assign('error', $e->getMessage());
		}
	}
}
elseif(isset($_GET['response'])){
	$smarty->assign('response', htmlentities($_GET['response']));
}

$smarty->display('tpl/hotelbook.html');