<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>IdentificationD</title>
	</head>
	<body>
		<form action="Detenues.php" method="POST">
			<fieldset>
				NOM:<input type="text" name="nomC_IdD" title="votre nom"/><br />
				PASS:<input type="text" name="numA_IdD" title="numéro d'abooné" /><br />
				<input type="submit" value="Envoyer"/>
			</fieldset>
		</form>
<?php
	banniere("IdentificationD",Auteur);
?>
	</body>
</html>
