<?php
$errorMessage = isset($_GET['reason']) ? $_GET['reason'] : "Unknown reason";
?>

<html>
	<head>
		<title>Papererror.php</title>
	</head>
	<body>
		<div class="new">An error has occurred: <span id="error"><?php echo $errorMessage;?></span></div>
	</body>
</html>
