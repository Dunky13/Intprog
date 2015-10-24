<html>
	<head>
		<title>Papers.php</title>
	</head>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
	<script
	$.ajax({
		method: "GET",
		url: "cgi-bin/papers.cgi",
		success: function(data){
			$(".paperlist").empty();
			$(data).each(function(index){
				var item = $("<li>").attr("class", "paper");
				var listID = $("<span>").attr("class", "id").text("["+this.id+"]");
				var listAuthor = $("<span>").attr("class", "author").text(this.author);
				var listTitle = $("<a>").attr("href", "cgi-bin/paperview.cgi?id="+this.id).attr("class", "title").text(this.title);
				item.append(listID).append(listAuthor).append(listTitle);
				$(".paperlist").append(item);
			});
		}
	});
	</script>
	<body>
		<ul class="paperlist">
		</ul>
	</body>
</html>
