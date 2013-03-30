<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>IdentificationC</title>
	</head>
	<body>
		<form action="Commande.php" method="POST">
			<fieldset>
				NOM:<input type="text" name="nomC_IdC" title="votre nom"/><br />
				PASS:<input type="text" name="numA_IdC" title="numéro d'abooné" /><br />
				<input type="submit" value="Envoyer"/>
			</fieldset>
		</form>
<?php
	banniere("IdentificationC",Auteur);
?>
	</body>
</html>
