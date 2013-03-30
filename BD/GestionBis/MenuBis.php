<style type='text/css'>
	.menuP{font-weight:bold;color:#ccc;} .menuP:hover{font-style:italic;color:red;}
</style>
<?php
	include "func.php";
	$check_ID=$_COOKIE["gestion_ID"];
	if ($check_ID!="estBon")
		echo "Rappel à l'ordre. <br />"
		."<meta http-equiv='refresh' content='2;url=indexBis.htm'>\n";
	else {
		init();
		
?>
<body>
	<a class='menuP' href="AccueilRetourBis.php">Retour de cassettes</a>
	<a class='menuP' href="">Enregistrer de nouveaux abonnés</a>
	<a class='menuP' href="">Modifier des fiches</a>
	<a class='menuP' href="">Radier des abonnés</a>
<body>
<?php
	}
?>

