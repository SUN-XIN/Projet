<?php
	include "Outils.inc";
	$link=DB_connect();
//	echo $link."<br />";
	$res1=DB_execSQL("select Nom from ABONNES where Code='1Xu123'",$link);
	$res2=DB_execSQL("select Titre from FILMS where NoFilm=1",$link);
	$res3=DB_execSQL("select NoFilm from CASSETTES where Statut='disponible'",$link);
	while ($nom=mysql_fetch_object($res1)) {
		echo $nom->Nom;
	}
	echo "<br />";
	while ($titre=mysql_fetch_object($res2)) {
		echo $titre->Titre;
	}
	echo "<br />";
	while ($s=mysql_fetch_object($res3)) {
		echo $s->NoFilm." ";
	}
	echo "<br />";
	
	$req="select f.NoFilm,f.Titre,c.Support from FILMS f,CASSETTES c where c.NoFilm=f.NoFilm and c.Statut='disponible' and (f.Titre like '%Chambre%')";
//	$req="select Nom from ABONNES where Code='1Xu123'";
	trans($req,$link);
	
	$req="select Titre from FILMS where NoFilm=1";
	$res=get_res($req,$link);
	echo $res;
	
	mysql_close($link);
?>
