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
					<label class="description" for="title">Titel </label>
					<div>
						<input id="title" class="element text medium" type="text" name="title" value="" />
					</div>
				</li>
				<li id="li_2">
					<label class="description" for="author">Author </label>
					<div>
						<input id="author" class="element text medium" type="text" name="author" value="" />
					</div>
				</li>
				<li id="li_3">
					<label class="description" for="file">Paper </label>
					<div>
						<input id="file" class="element file" name="file" type="file" />
					</div>
				</li>

				<li class="buttons">
					<input id="saveForm" type="submit" name="submit" value="Upload paper" />
				</li>
			</ul>
		</form>
	</body>

	</html>
