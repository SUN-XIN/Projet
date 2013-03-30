<?php
	define("Auteur","SUN Xin");
	function banniere($page,$nom){
		echo "<h1>VideoEXPRESS</h1>";
		echo "<p>&#169;".$nom."</p>";
		echo "<a href='Accueil' title='La page d&#39;Accueil'>Accueil</a><br /><br />";
		if($page!="AccueilDescriptif")
			echo "<a href='AccueilDescriptif.php' title='Descriptif d&#39;un film'>Descriptif d'un film</a><br />";
		if($page!="AccueilRecherche")
			echo "<a href='AccueilRecherche.php' title='Recherche de films'>Recherche de films</a><br />";
		if($page!="IdentificationC")
			echo "<a href='IdentificationC.php' title='Commande de cassettes'>Commande de cassettes</a><br />";
		if($page!="IdentificationD")
			echo "<a href='IdentificationD.php' title='Liste des cassette détenues'>Liste des cassette détenues</a><br />";
	}
	
	function DB_connect(){
		$link=mysql_connect("poux.ufr-info-p6.jussieu.fr", "video44", "4822");
		if (!$link) {
			die('Connexion impossible : '.mysql_error());
		}
		echo 'Connexion réussie';
		mysql_close($link);
	}
	
	function DB_execSQL($req,$serv){
	}
?>
