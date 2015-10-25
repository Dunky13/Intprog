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
$smarty->assign('cgibase', $WEB_BASECGI);
$smarty->assign('new', isset($_GET['new']));
$smarty->assign('paperID', isset($_GET['id']));

$smarty->display('tpl/papers.html');
