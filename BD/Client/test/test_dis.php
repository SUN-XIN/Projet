<?php
	include "Outils.inc";
	$link=DB_connect();
	$req="select distinct Realisateur from FILMS";
	$res=DB_execSQL($req,$link);
	while ($r=mysql_fetch_object($res)) {
		$n=$r->Realisateur;
		echo $n."<br />";
	}
	mysql_close($link);
?>
