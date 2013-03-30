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
					<option>Comédie</option>
					<option>Comédie dramatique</option>
					<option>Drame</option>
					<option selected>indifférent</option>
				</select><br />
				Nom de réalisateur:
				<select name="realisateurARe" class="posit">
					<option>François Dupeyron</option>
					<option>Luis Bunuel</option>
					<option>Manuel Poirier</option>
					<option>Krzysztof Kieslowski</option>
					<option>Jacques Doillon</option>
					<option selected>indifférent</option>
				</select><br />
				Nom de acteur:
				<select name="acteurARe" class="posit">
					<option>Eric Caravaca</option>
					<option>Sabine Azéma</option>
					<option>André Dussollier</option>
					<option>Fernando Rey</option>
					<option>Paul Frankeur</option>
					<option selected>indifférent</option>
				</select><br />
				<!--   *************************************************   -->
				<input type="submit" value="Envoyer"/>
			</fieldset>
			</div>
		</form>
<?php
	banniere("AccueilRecherche",Auteur);
?>
	</body>
</html>
