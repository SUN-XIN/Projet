<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>IdentificationC</title>
	</head>
	<body>
<?php
	banniere("IdentificationC",Auteur);
?>
		<h2>Commande de cassettes</h2>
		<CENTER>
		<form action="Commande.php" method="POST">
			<fieldset>
				NOM:<input type="text" name="nomC_IdC" title="votre nom"/><br />
				PASS:<input type="text" name="numA_IdC" title="numéro d'abooné" /><br />
				<input type="submit" value="Envoyer"/>
			</fieldset>
		</form>
		</CENTER>
	</body>
</html>
