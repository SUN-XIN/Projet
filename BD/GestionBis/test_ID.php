<?php
	if (isset($_POST["nomAD"]) && isset($_POST["passAD"])) {
		if (($_POST["nomAD"]=="bond") && ($_POST["passAD"]=="007")) {
			setcookie("gestion_ID","estBon");
			echo "Ordre permet. \n"
			."<meta http-equiv='refresh' content='2;url=MenuBis.php'>\n";
		}
		else echo "Rappel à l’ordre. ".
		"<meta http-equiv='refresh' content='2;url=indexBis.htm'>\n";
		
	}
	else echo "Il faut saisir le nom d'administration et mot de passe";
?>
