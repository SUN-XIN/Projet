<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>Recherche</title>
		<style type="text/css">
			.c1{color:red;}
			.c9{color:green;}
		</style>
	</head>
	<body>
<?php
	banniere("AccueilRecherche",Auteur);
	//$_POST["titreF_ARe"]	Titre de film
	//$_POST["supportARe"]	Support
	//$_POST["dispoARe"]	Disponibilité
	//$_POST["genreARe"]	Genre
	//$_POST["realisateurARe"]	Nom de réalisateur
	//$_POST["acteurARe"]	Nom de acteur
	$req="select f.NoFilm,f.Titre,c.Support from FILMS f,CASSETTES c";
	$tabReq="";
	$condReq="where c.NoFilm=f.NoFilm";
	//pour TABLE:FILMS
	if ($_POST["realisateurARe"]!="indifférent") $condReq.=" and f.Realisateur='".$_POST["realisateurARe"]."'";
	if ($_POST["genreARe"]!="indifférent") $condReq.=" and f.genre='".$_POST["genreARe"]."'";
	
	//pour TABLE:ACTEURS
	if ($_POST["acteurARe"]!="indifférent") {
		$tabReq.=",ACTEURS a";
		$condReq.=" and a.NoFilm=f.NoFilm and a.Acteur='".$_POST["acteurARe"]."'";
	}
	//pour TABLE:CASSETTES
		if ($_POST["supportARe"]!="indifférent") {
			$condReq.=" and c.Support='".$_POST["supportARe"]."'";
		}
		if ($_POST["dispoARe"]!="indifférent") {
			$condReq.=" and c.Statut='disponible'";
		}
	
	
	if (trim($_POST["titreF_ARe"])!="") {
		$tabM=explode(" ",$_POST["titreF_ARe"]);
		//preg_split
		if (count($tabM)>0) $condReq.=" and (";
		foreach ($tabM as $t) {
			if ($t==$tabM[0]) $condReq.="f.Titre like '%$t%'";
			else $condReq.=" or f.Titre like '%$t%'";
		}
		if (count($tabM)>0) $condReq.=")";
		}
	$req.=$tabReq." ".$condReq;
//	echo "Commonde SQL : ".$req."<br />";
	$link=DB_connect();
	trans($req,$link);
?>
	</body>
</html>
