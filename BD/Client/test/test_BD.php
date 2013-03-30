<?php
	include "Outils.inc";
	$link=DB_connect();
	$n=get_res("select NbCassettes from ABONNES where Code='1Xu123'",$link);
	$n++;
	echo "*********".$n."*********<br />";
	echo "<br />";

	mysql_close($link);
?>

<?php
$date = date_create('2012-05-05 15:24:35');
date_add($date, date_interval_create_from_date_string('10 minute'));
$is_now=date("Y-m-d H-i-s");
echo time();
echo date_format($date, 'Y-m-d h-m-s');
echo strtotime(date_format($date, 'Y-m-d h-m-s'));
?>
