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
$smarty->assign('new', isset($_GET['new']));

$smarty->display('tpl/papers.html');

<?php
include("config.php");
?>

<html>
	<head>
		<title>Paperinfo.php</title>
	</head>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<script>
	$(document).ready(function(){
		var isNew = <?php echo isset($_GET['new']) ? "true" : "false"; ?>;
		if(isNew){$(".new").show();}
		else	{$(".new").hide();}
		$.ajax({
			method: "GET",
			url: "<?php echo $WEB_BASECGI;?>/paperinfo.cgi?id=<?php echo $_GET["id"];?>",
			success: function(data){
				$(".paperlist").empty();
				var item = $("<li>").attr("class", "paper");
				var listID = $("<span>").attr("class", "id").text("["+data.id+"]");
				var listAuthor = $("<span>").attr("class", "author").text(data.author);
				var listTitle = $("<a>").attr("href", "cgi-bin/paperview.cgi?id="+data.id).attr("class", "title").text(data.title);
				item.append(listID).append(listAuthor).append(listTitle);
				$(".paperlist").append(item);
			}
		});
	
	});
	</script>
	<body>
		<span class="new">The paper has succesfully uploaded, this is the paper's data:</span>
		<ul class="paperlist"></ul>
		<a href="<?php echo $WEB_BASEPHP; ?>">Go Back Home</a>
	</body>
</html>
