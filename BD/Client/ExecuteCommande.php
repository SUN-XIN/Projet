<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>ExecuteCommande</title>
		<style type="text/css">
		</style>
	</head>
	<body>
<?php
	banniere("IdentificationC",Auteur);
	$link=DB_connect();

//	$_POST['PASS'] numéro d'abonné
//	$_POST['NOM'] nom
//	$_POST['Exi'] numéro d'examplaire
//	$_POST['NumFilmi'] numéro du film
//	$_POST['Supporti'] support
//	$_POST['NBR'] nombre de case cochée
	$depasser=false;
	$nom_t=$_POST["NOM"];
	$pass_t=$_POST["PASS"];
	$nb_t=$_COOKIE["nbr_final"];
	for ($k=0;$k<$nb_t;$k++) {
		$nom1="NumFilm".$k;
		$nom1=$_POST["$nom1"];
		$nom2="Support".$k;
		$nom2=$_POST["$nom2"];
		$nom3="Ex".$k;
		$nom3=$_POST["$nom3"];

		$req="select CodeAbonne from EMPRES where NoFilm=$nom1 and NoExemplaire=$nom3 and DATE_ADD(DateEmpRes,INTERVAL 5 SECOND)>NOW()";
//		$req="select CodeAbonne from EMPRES where NoFilm=$nom1 and NoExemplaire=$nom3 and DATE_ADD(DateEmpRes,INTERVAL 5 MINUTE)>NOW()";
		$is_OK="";
		$is_OK=get_res($req,$link);
		if ($is_OK=="") {
			$depasser=true;
			echo "Il est Dépassé le temps limit(5 minutes). Merci de votre compréhension.";

//			supprimer d'1 ligne dans EMPRES
			$req="delete from EMPRES where NoFilm=$nom1 and NoExemplaire=$nom3";
			$bd=DB_execSQL($req,$link);
//			colonne Statut de CASSETTES mise à 'disponible'
			$req="update CASSETTES set Statut='disponible' where NoFilm=$nom1 and NoExemplaire=$nom3";
			$bd=DB_execSQL($req,$link);

			echo "<CENTER><form action='Commande.php' method='POST'><fieldset>";
			echo "<label>Retourner à la page Commande pour rechoisir.</label><br />";
			echo "<input type='hidden' name='nomC_IdC' value='$nom_t' />";
			echo "<input type='hidden' name='numA_IdC' value='$pass_t' />";
			echo "<input type='submit' value='Revoir le choix' />";
			echo "</fieldset></form></CENTER>";
		}
		else {

//mettre a jour CASSETTES:colonne Statut
//		ABONNES:colonne NbCassettes
//		EMPRES
//		*******************
		$req="update CASSETTES set Statut='empruntee' where NoFilm=".$nom1." and NoExemplaire=".$nom3;
		$bd=DB_execSQL($req,$link);

		$req="update EMPRES set DateEmpRes=NOW() where NoFilm=".$nom1." and NoExemplaire=".$nom3;
		$bd=DB_execSQL($req,$link);
//		*******************
		}
	}
	if (!$depasser) {
		$n=get_res("select NbCassettes from ABONNES where Code='".$pass_t."'",$link);
		$n=$n+$nb_t;
		$req="update ABONNES set NbCassettes=$n where Code='".$pass_t."'";
		$bd=DB_execSQL($req,$link);
		$n=get_res("select NbCassettes from ABONNES where Code='".$pass_t."'",$link);
		echo "Votre commande est bien enregistrée.";
	}

	mysql_close($link);
?>
	</body>
</html>
