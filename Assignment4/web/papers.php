<html>
	<head>
		<title>Papers.php</title>
	</head>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<script>
	$.ajax({
		url: "cgi-bin/papers.cgi",
		success: function(data){
			console.log(data);
		}
	});
	</script>
	<body>
	Hi
	</body>
</html>
