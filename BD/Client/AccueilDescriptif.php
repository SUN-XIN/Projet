<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>AccueilDescriptif</title>
	</head>
	<body>
<?php
	banniere("AccueilDescriptif",Auteur);
?>
		<h2>Descriptif d'un film</h2>
		<CENTER>
		<form action="Descriptif.php" method="POST">
			<fieldset>
				Numéro de film:<input type="text" name="numF_ADes"/><br />
				<input type="submit" value="Envoyer"/>
			</fieldset>
		</form>
		</CENTER>
	</body>
</html>
