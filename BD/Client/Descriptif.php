<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>Descriptif</title>
		<style type="text/css">
			.posit{position: absolute; left: 200px;}
			#pad{line-height:30px}
		</style>
	</head>
	<body>
<?php
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
		
		
		/* 
		select Titre from FILMS where NoFilm=$_POST["numF_ADes"];
		select Nationalite from FILMS where NoFilm=$_POST["numF_ADes"];
		select Realisateur from FILMS where NoFilm=$_POST["numF_ADes"];
		select Annee from FILMS where NoFilm=$_POST["numF_ADes"];
		select Couleur from FILMS where NoFilm=$_POST["numF_ADes"];
		select Duree from FILMS where NoFilm=$_POST["numF_ADes"];
		select Synopsis from FILMS where NoFilm=$_POST["numF_ADes"];
		select Genre from FILMS where NoFilm=$_POST["numF_ADes"];
		select Acteur from ACTEURS where NoFilm=$_POST["numF_ADes"];
		*/

	banniere("AccueilDescriptif",Auteur);
?>
	</body>
</html>
