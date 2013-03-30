<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>Descriptif</title>
		<style type="text/css">
		</style>
	</head>
	<body>
<?php
	banniere("AccueilDescriptif",Auteur);
		/*
		echo "<p>Numéro du film : ".$_POST["numF_ADes"]."</p>";
		echo "<p>Titre : "."</p>";
		echo "<p>Nationalité : "."</p>";
		echo "<p>Réalisateur : "."</p>";
		echo "<p>Année de production : "."</p>";
		echo "<p>Couleur : "."</p>";
		echo "<p>Durée : "."</p>";
		echo "<p>Résumé : "."</p>";
		echo "<p>Genre : "."</p>";
		echo "<p>Liste des principaux acteurs : "."</p>";
		echo "<br /><br /><br />";
		
		"select Titre from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Nationalite from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Realisateur from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Annee from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Couleur from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Duree from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Synopsis from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Genre from FILMS where NoFilm=$_POST['numF_ADes']";
		"select Acteur from ACTEURS where NoFilm=$_POST['numF_ADes']";
		*/
		
		$link=DB_connect();
		$req="select f.Titre from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Nationalite from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Realisateur from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Annee from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Couleur from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Duree from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Synopsis from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select f.Genre from FILMS f where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);
		$req="select a.Acteur from ACTEURS a where NoFilm=".$_POST["numF_ADes"];
		trans($req,$link);

		mysql_close($link);		
?>
	</body>
</html>
