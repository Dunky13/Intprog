<?php
include("config.php");
?>

	<html>

	<head>
		<title>Paperload.php</title>
		<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	</head>
	<body>
		<form enctype="multipart/form-data" method="post" action="<?php echo $WEB_BASECGI;?>/paperload.cgi">
			<ul>

				<li>
					<label class="description" for="element_1">Titel </label>
					<div>
						<input id="element_1" name="element_1" class="element text medium" type="text" name="title" value="" />
					</div>
				</li>
				<li id="li_2">
					<label class="description" for="element_2">Author </label>
					<div>
						<input id="element_2" name="element_2" class="element text medium" type="text" name="author" value="" />
					</div>
				</li>
				<li id="li_3">
					<label class="description" for="element_3">Paper </label>
					<div>
						<input id="element_3" name="element_3" class="element file" name="file" type="file" />
					</div>
				</li>

				<li class="buttons">
					<input id="saveForm" type="submit" name="submit" value="Upload paper" />
				</li>
			</ul>
		</form>
	</body>

	</html>
