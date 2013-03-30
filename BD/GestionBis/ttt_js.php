<html>
	<head>
<script type="text/javascript">
	function innec() {
		document.cookie="estBon=NON";
		document.write("Innection succès. "+
		"<meta http-equiv='refresh' content='2;url=indexBis.htm'>\n";
	}
	function init(id) {
		alert(id);
		document.getElementById(id).innerHTML="<input type='button' value='Innection' onclick='innec()' />";
	}
</script>
	</head>
<body>
	<div id="innection"></div>
<?php
	echo "<input type='button' value='Innection' onclick='innec()' />";
	echo "***".$_COOKIE["gestion_ID"]."****";
//	setcookie("gestion_ID","NON");
?>
</body>
</html>
