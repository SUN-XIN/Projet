<style type='text/css'>
	#out {position:relative;left:80%;} #out:hover{font-style:italic;color:blue;}
	#mode1 {font-weight:bold;color:red;}
	.mode2 {color:white;} .mode2:hover{font-style:italic;color:black;}
	body {background-image:url("1.jpg")}
</style>
<script type="text/javascript">
	function innec() {
		document.cookie="gestion_ID=NON";
//		document.write="Innection Succès. ";
		alert('Innection Succès !');
		window.location="indexBis.htm";
	}
</script>

<?php
	function init(){
		echo "<h id='mode1'>C'est en mode de GESTION ! </h><br />";
		echo "<input id='out' type='button' value='Innection' onclick='innec()' /><br />";
		echo "<a class='mode2' href='MenuBis.php'>Menu Principal</a><br />";
		echo "<a class='mode2' href='../Client/Accueil.php'>Session Client</a><br /><br />";
	}
?>
