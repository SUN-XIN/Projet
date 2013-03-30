<?php
	if (isset($_POST["nomAD"]) && isset($_POST["passAD"])) {
		if (($_POST["nomAD"]=="bond") && ($_POST["passAD"]=="007")) {
			echo "Ordre permet. <br />";
?>
	<a href="AccueilRetour.php">Retour de cassettes</a>
	<a href="">Enregistrer de nouveaux abonnés</a>
	<a href="">Modifier des fiches</a>
	<a href="">Radier des abonnés</a>
<?php
		}
		else echo "Rappel à l’ordre. ";
	}
	else echo "Il faut saisir le nom d'administration et mot de passe";
?>
