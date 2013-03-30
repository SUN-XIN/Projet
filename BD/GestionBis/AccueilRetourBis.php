<html>
	<head>
		<title>AccueilRetour</title>
	</head>
	<body>
<?php
	include "func.php";
	init();
?>
		<h2>Retour de cassettes</h2>
		<form action="RetourBis.php" method="POST">
			<fieldset>
				Numéro de film:<input type="text" name="numF_AR"/><br/>
				Numero d'exemplaire:<input type="text" name="numE_AR" title="numéro d'exemplaire de la cassette à rentrer" /><br/>
				<input type="submit" value="Envoyer"/>
			</fieldset>
		</form>
	</body>
</html>
