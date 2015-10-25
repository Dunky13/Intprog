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
	</body>
</html>
