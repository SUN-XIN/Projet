<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>Detenues</title>
		<style type="text/css">
			.esp1 {position:absolute;left:35%;}
			.esp2 {position:absolute;left:70%;}
		</style>
	</head>
	<body>
<?php
	banniere("IdentificationD",Auteur);
	$link=DB_connect();
//	$_POST["nomC_IdD"] votre nom
//	$_POST["numA_IdD"] numéro d'abooné
	$r_c="";	
	$r_c.=get_res("select Code from ABONNES where Nom='".$_POST["nomC_IdD"]."'",$link);
	if ($r_c!="") {
		if ($_POST["numA_IdD"]==$r_c) {
			echo "<h>Bienvenu ".$_POST["nomC_IdD"]." !</h><br />";
			$req="select NbCassettes from ABONNES where Nom='".$_POST["nomC_IdD"]."'";
			$nb_emp=get_res($req,$link);
			if ($nb_emp==0)
				echo "Vous n'avez aucun emprunt";
			else {
				echo "<table border=1><tr><th>Contenu</th>";
				for ($i=1;$i<=$nb_emp;$i++) {
				echo "<th>Film NO.".$i."</th>";
				}
				echo "</tr>";
//				Champs "Numéro de film"
				$req="select NoFilm from EMPRES where CodeAbonne='".$r_c."'";
				fun_tab($req,$link,"NoFilm");
//				Champs "Numéro d'exemplaire"
				$req="select NoExemplaire from EMPRES where CodeAbonne='".$r_c."'";
				fun_tab($req,$link,"NoExemplaire");
//				Champs "Le titre"
				$req="select f.Titre from FILMS f,EMPRES e where e.CodeAbonne='".$r_c."' and e.NoFilm=f.NoFilm";
				fun_tab($req,$link,"Titre");
//				Champs "Réalisateur"
				$req="select f.Realisateur from FILMS f,EMPRES e where e.CodeAbonne='".$r_c."' and e.NoFilm=f.NoFilm";
				fun_tab($req,$link,"Realisateur");
				echo "</table>";
			}
		}
		else {
			echo "Votre Mot de passe n'est pas correct. <br />";
			echo "<a href='IdentificationD.php' title='Liste des cassette détenues' />Retourner à la page precedente.</a>";
		}
	}
	else {
		echo "On ne trouve pas votre dossier. Vérifiez-vous votre Nom.<br />";
		echo "<a href='IdentificationD.php' title='Liste des cassette détenues' />Retourner à la page precedente.</a>";
		//echo "<script language='javascript'>window.location='IdentificationD.php';</script>";
	}
	mysql_close($link);
?>
	</body>
</html>
