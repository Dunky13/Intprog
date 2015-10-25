<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
include("config.php");
?>

<html>
	<head>
		<title>Paperinfo.php</title>
	</head>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<script>
	$(document).ready(function(){
		var isNew = <?php echo isset($_GET['new'] ? "true" : "false") ?>;
		$.ajax({
			method: "GET",
			url: "<?php echo $WEB_BASECGI;?>/paperinfo.cgi?id=<?php echo $_GET["id"];?>",
			success: function(data){
				$(".paperlist").empty();
				//$(data).each(function(index){
					var item = $("<li>").attr("class", "paper");
					var listID = $("<span>").attr("class", "id").text("["+this.id+"]");
					var listAuthor = $("<span>").attr("class", "author").text(this.author);
					var listTitle = $("<a>").attr("href", "cgi-bin/paperview.cgi?id="+this.id).attr("class", "title").text(this.title);
					item.append(listID).append(listAuthor).append(listTitle);
					$(".paperlist").append(item);
				//});
			}
		});
	
	});
	</script>
	<body>
		<ul class="paperlist">
		</ul>
	</body>
</html>
