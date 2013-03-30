<html>
	<head>
		<title>Retour</title>
	</head>
	<body>
<?php
	include "../Client/Outils.inc";
	$link=DB_connect();
//	$_POST[numF_AR] Numéro de film
//	$_POST[numE_AR] numéro d'exemplaire de la cassette à rentrer
	$res="";
	$req="select Support from CASSETTES where NoFilm=".$_POST["numF_AR"]." and NoExemplaire=".$_POST["numE_AR"];
	$res.=get_res($req,$link);
	if ($res!="") {
		$req="select CodeAbonne from EMPRES where NoFilm=".$_POST["numF_AR"]." and NoExemplaire=".$_POST["numE_AR"];
		$codeA=get_res("select CodeAbonne from EMPRES where NoFilm=".$_POST["numF_AR"]." and NoExemplaire=".$_POST["numE_AR"],$link);
		$req="update CASSETTES set Statut='disponible' where NoFilm=".$_POST["numF_AR"]." and NoExemplaire=".$_POST["numE_AR"];
		$bd=DB_execSQL($req,$link);
		$nbrC=get_res("select NbCassettes from ABONNES where Code='".$codeA."'",$link);
		$nbrC--;
		$req="update ABONNES set NbCassettes=$nbrC where Code='".$codeA."'";
		$bd=DB_execSQL($req,$link);
		$req="delete from EMPRES where NoFilm=".$_POST["numF_AR"]." and NoExemplaire=".$_POST["numE_AR"];
		$bd=DB_execSQL($req,$link);
		echo "La cassette dont numéro exemplaire : ".$_POST["numE_AR"]." du film numéro : ".$_POST["numF_AR"]." est bien retournée .<br />";
		echo "<a href='AccueilRetour.php' title='AccueilRetour'>AccueilRetour</a><br />";
		echo "<a href='index.htm' title='Gestion'>Session Gestion</a>";
	}
	else {
		echo "Erreur de saisir. Cassette concernée n'est pas trouvée.<br />";
		echo "<a href='AccueilRetour.php' title='AccueilRetour'>Retourner à la page precedente.</a><br />";
		echo "<a href='index.htm' title='Gestion'>Session Gestion</a>";
	}
	mysql_close($link);
?>
	</body>
</html>
