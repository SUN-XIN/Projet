<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>AccueilRecherche</title>
		<style type="text/css">
			.posit{position: absolute; left: 200px;}
			#pad{line-height:30px}
		</style>
	</head>
	<body>
<?php
	banniere("AccueilRecherche",Auteur);
	$link=DB_connect();
?>
		<h2>Recheche de films</h2>
		<form action="Recherche.php" method="POST">
			<div id="pad">
			<fieldset>
				Titre de film:<input type="text" name="titreF_ARe"/><br />
				Support:
				<select name="supportARe" class="posit">
					<option>DVD</option>
					<option>VHS</option>
					<option selected>indifférent</option>
				</select><br />
				Disponibilité:
				<select name="dispoARe" class="posit">
					<option selected>disponible</option>
					<option>indifférent</option>
				</select><br />
				<!--   chercher la base de données   -->
				<!--   *************************************************   -->
				Genre:
				<select name="genreARe" class="posit">
<?php 
	$req="select distinct Genre from FILMS";
	$res=DB_execSQL($req,$link);
	while ($r=mysql_fetch_object($res)) {
		$n=$r->Genre;
		echo "<option>".$n."</option>";
	}
?>
					<option selected>indifférent</option>
				</select><br />
				Nom de réalisateur:
				<select name="realisateurARe" class="posit">
<?php 
	$req="select distinct Realisateur from FILMS ORDER BY Realisateur DESC";
	$res=DB_execSQL($req,$link);
	while ($r=mysql_fetch_object($res)) {
		$n=$r->Realisateur;
		echo "<option>".$n."</option>";
	}
?>
				<option selected>indifférent</option>
				</select><br />
				Nom de acteur:
				<select name="acteurARe" class="posit">
<?php 
	$req="select distinct Acteur from ACTEURS ORDER BY Acteur DESC";
	$res=DB_execSQL($req,$link);
	while ($r=mysql_fetch_object($res)) {
		$n=$r->Acteur;
		echo "<option>".$n."</option>";
	}
?>
					<option selected>indifférent</option>
				</select><br />
				<!--   *************************************************   -->
				<input type="submit" value="Envoyer"/>
			</fieldset>
			</div>
		</form>
<?php mysql_close($link) ?>
	</body>
</html>
