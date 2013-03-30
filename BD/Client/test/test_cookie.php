<?php
	setcookie("nbr",9);
?>
<script>
function le_cookie(nom) {
	var arrStr = document.cookie.split("; ");
	for(var i = 0;i < arrStr.length;i ++){
		var temp = arrStr[i].split("=");
		if(temp[0] == nom) return unescape(temp[1]);
	} 
}

function checkit() {
	var nbr_vide=0;
	var nbr_total=le_cookie("nbr");
	for(var i=1;i<3;i++) {
//		if(document.getElementById("Support"+i).checked) chk=true;
		if (document.getElementById("NumFilm"+i).value=="") nbr_vide++;
	}
	var nbr_final="nbr="+(nbr_total-nbr_vide);
	document.cookie=nbr_final;
	alert(nbr_final);
}
</script>
<form action="test_r_cookie.php">
	NumFilm1:<input type="text" id="NumFilm1">
	NumFilm2:<input type="text" id="NumFilm2">
	<input type=button value="button" onclick="checkit()">
	<input type="submit" value="OK">
</form>

