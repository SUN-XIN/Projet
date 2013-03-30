<?php
	define("Auteur","SUN Xin");
	function banniere($page,$nom){
		echo "<style type='text/css'>";
		echo "a {color:blue;}";
		echo ".ac{font-weight:bold;color:#ccc;} .ac:hover{font-style:italic;color:red;}";
		echo "a:hover{font-weight:bold;color:orange}";
		echo "p {font-style:italic}";
		echo "h1 {color:green;text-align:center;;font-style:italic;}";
		echo "#me {position:relative;left:550px;}";
		echo "body {background-image:url('2.jpg')}";
		echo "</style>";

		echo "<h1>VideoEXPRESS</h1>";
		echo "<p id='me'>&#169;".$nom."</p>";
		echo "<a class='ac' href='Accueil' title='La page d&#39;Accueil'>Accueil</a><br /><br />";
		echo "<a class='ac' href='../Gestion/index.htm' title='Gestion'>Session Gestion<a><br />";
//		echo "<a class='ac' href='../GestionBis/indexBis.htm' title='Gestion'>Session Gestion<a><br />";
		echo "<ul>";
		if($page!="AccueilDescriptif")
			echo "<li><a href='AccueilDescriptif.php' title='Descriptif d&#39;un film'>Descriptif d'un film</a></li>";
		if($page!="AccueilRecherche")
			echo "<li><a href='AccueilRecherche.php' title='Recherche de films'>Recherche de films</a></li>";
		if($page!="IdentificationC")
			echo "<li><a href='IdentificationC.php' title='Commande de cassettes'>Commande de cassettes</a></li>";
		if($page!="IdentificationD")
			echo "<li><a href='IdentificationD.php' title='Liste des cassette détenues'>Liste des cassette détenues</a></li>";
		echo "</ul>";
		echo "<hr /> ";
	}
/*	
	function DB_connect(){
		$link=mysql_connect("localhost", "sx", "2222");
		if (!$link) {
			echo("Connexion impossible : ".mysql_error());
			return 0;
		}
		echo "Connexion réussie.<br />";
		$db_selected=mysql_select_db("videoBis", $link);
		if (!$db_selected) {
			echo ("Can't use sx : ".mysql_error());
			return 0;
		}
		return $link;
//		mysql_close($link);
	}
*/
function DB_connect(){
		$link=mysql_connect("localhost", "video44", "4822");
		if (!$link) {
			echo("Connexion impossible : ".mysql_error());
			return 0;
		}
		echo "Connexion réussie.<br />";
		$db_selected=mysql_select_db("video44", $link);
		if (!$db_selected) {
			echo ("Can't use video44 : ".mysql_error());
			return 0;
		}
		return $link;
//		mysql_close($link);
	}
	
	function DB_execSQL($req,$serv){
		$result=mysql_query($req,$serv);
		if ($result) {
			return $result;
		}
		else {
//			echo "Error creating database: ".mysql_error();
			echo "Votre cherche n'est pas trouvé.";
			return 0;
		}
	}
	
	function trans($req,$serv) {
		$tab=explode("from",$req);
		$tab=explode(",",$tab[0]);
		$nb_select=count($tab);
//		echo "Nombre element de select : ".$nb_select."<br />";
		for ($i=0;$i<$nb_select;$i++) {
			$t=explode(".",$tab[$i]);
			$select[$i]=$t[1];
//			echo $i." est ".$t[1]."<br />";
		}
		$res=DB_execSQL($req,$serv);
		if ($res) {
			$mark_a=false;
			while ($r=mysql_fetch_object($res)) {
				foreach ($select as $s) {
					$s=trim($s);
					switch ($s) {
						case "NoFilm":		echo "<a class='c1'>Numéro du film : ";
											break;
						case "Nationalite":	echo "<a class='c2'>Nationalité : ";
											break;
						case "Realisateur":	echo "<a class='c3'>Réalisateur : ";
											break;
						case "Annee":		echo "<a class='c4'>Année de production : ";
											break;
						case "Duree":		echo "<a class='c5'>Durée(mins) : ";
											break;
						case "Synopsis":	echo "<a class='c6'>Résumé : ";
											break;
						case "Acteur":		if (!$mark_a) {
												echo "<a class='c7'>Liste des principaux acteurs : <br />";
												$mark_a=true;
											}
											else echo "<a class='c8'><br />";
											break;
						default:			echo "<a class='c9'>".$s." : ";
											break;
					}
					echo $r->$s." </a>";
				}
				echo "<br />";
			}
		}
	}
	
	function get_res($req,$serv) {
		$tab=explode("from",$req);
		$tab=explode(" ",$tab[0]);
		$t=trim($tab[1]);
		$res=DB_execSQL($req,$serv);
		while ($r=mysql_fetch_object($res)) {
			$n=$r->$t;
		}
		return $n;
	}

	function fun_tab($req,$serv,$select) {
//		Champs "$select"
		echo "<tr><td>";
		switch ($select) {
			case "NoFilm":	echo "Numéro de film";
					break;
			case "NoExemplaire":	echo "Numéro d'exemplaire";
					break;
			case "Titre":	echo "Le titre";
					break;
			case "Realisateur":	echo "Réalisateur";
					break;
		}
		echo "</td>";
		$res=DB_execSQL($req,$serv);
		while ($r=mysql_fetch_object($res)) {
			$n=$r->$select;
			echo "<td>$n</td>";
		}
		echo "</tr>";
	}
?>
