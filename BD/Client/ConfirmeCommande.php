<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>ConfirmeCommonde</title>
		<style type="text/css">
		</style>
	</head>
	<body>
<?php
	banniere("IdentificationC",Auteur);
	$link=DB_connect();

//	$_POST['PASS'] numéro d'abonné
//	$_POST['NOM'] nom
//	$_POST['NBR'] nombre de case cochée
//	$_POST['NumFilmi'] numéro du film
//	$_POST['Supporti'] DVD/VHS

	$nom_t=$_POST["NOM"];
	$pass_t=$_POST["PASS"];
	$nbr_final=$_COOKIE["nbr"];
	setcookie("nbr_final",$nbr_final);
	echo "<form action='ExecuteCommande.php' method='POST'>"; 
	echo "<table border=1>";
	echo "<tr><th>Numéro du Film</th><th>Titre du Film</th><th>Disponibilité du Film</th><th>Commander</th></tr>";
	$dispo=false;
	$i=0;
	for ($k=0;$k<$nbr_final;$k++){
		$nom1="NumFilm".$k;
		$nom1=$_POST["$nom1"];
		$nom2="Support".$k;
		$nom2=$_POST["$nom2"];
		$t_f=get_res("select Titre from FILMS where NoFilm=".$nom1,$link);
		$d_f=get_res("select Statut from CASSETTES where NoFilm=$nom1 and Support='".$nom2."'",$link);
		if ($nom2=="DVD")		
			$d_f_bis=get_res("select Statut from CASSETTES where NoFilm=$nom1 and Support='VHS'",$link);
		else
			$d_f_bis=get_res("select Statut from CASSETTES where NoFilm=$nom1 and Support='DVD'",$link);
		echo "<tr><td>$nom1</td><td>$t_f</td><td>";
//			******************************
//			Ne pouvoir pas dépasser 5 mins
			if ($d_f=="reservee") {
				$n_e=get_res("select NoExemplaire from CASSETTES where NoFilm=$nom1 and Support='$nom2'",$link);
				$req="select CodeAbonne from EMPRES where NoFilm=$nom1 and NoExemplaire=$n_e and DATE_ADD(DateEmpRes,INTERVAL 5 SECOND)<NOW()";
//				$req="select CodeAbonne from EMPRES where NoFilm=$nom1 and NoExemplaire=$n_e and DATE_ADD(DateEmpRes,INTERVAL 5 MINUTE)<NOW()";
				$is_OK="";
				$is_OK=get_res($req,$link);
				if ($is_OK!="") {
//					supprimer d'1 ligne dans EMPRES
					$req="delete from EMPRES where NoFilm=$nom1 and NoExemplaire=$n_e";
					$bd=DB_execSQL($req,$link);
//					colonne Statut de CASSETTES mise à 'disponible'
					$req="update CASSETTES set Statut='disponible' where NoFilm=$nom1 and NoExemplaire=$n_e";
					$bd=DB_execSQL($req,$link);
					$d_f="disponible";
				}
			}
//			******************************
//		Champs "Disponibilité du Film"
		if ($d_f=="disponible") {
			echo "oui";
			$dispo=true;
		}
		else
			if ($d_f_bis=="disponible")
				if ($_POST["$nom2"]="DVD")
					echo "VHS";
				else
					echo "DVD";
			else
				echo "non";
		echo "</td><td>";
//		Champs "Commander"
		if ($dispo) {
			$n_e=get_res("select NoExemplaire from CASSETTES where NoFilm=$nom1 and Support='$nom2'",$link);
			if ($d_f=="disponible") {
				$nom1n="NumFilm".$i;
				$nom2n="Support".$i;
				$nom3n="Ex".$i;
				$nom4n="commande".$i;
				echo "<input type='checkbox' name='$nom4n' value='true' checked='checked' onchange='to_check(this)'/>";
//				colonne Statut de CASSETTES mise à 'reservee'
				$req="update CASSETTES set Statut='reservee' where NoFilm=$nom1 and NoExemplaire=$n_e";
				$bd=DB_execSQL($req,$link);

//				ajouter d'1 ligne dans EMPRES
				$req="INSERT INTO EMPRES VALUES($nom1,$n_e,'".$pass_t."',NOW())";
				$bd=DB_execSQL($req,$link);

//				le numéro d'examplaire est transmis par 1 champ caché nommé Exi
				echo "<input type='hidden' name='$nom3n' value='$n_e' />";

				echo "<input type='hidden' name='$nom1n' value='$nom1' />";
				echo "<input type='hidden' name='$nom2n' value='$nom2' />";
				$i++;
			}
		}
		else echo "Aucun film demandé n'est pas disponible. ";
		echo "</td></tr>";
	}
	echo "</table>";
		//le numéro d'abonné est transmis par 1 champ caché nommé PASS
		echo "<input type='hidden' name='PASS' value='$pass_t' />";
		//le nombre de case cochée est transmis par 1 champ caché nommé NBR
		echo "<input type='hidden' name='NBR' value='$i' />";
		//bouton "Commander"
		echo "<input type='hidden' name='NOM' value='$nom_t' />";
	if ($dispo) echo "<input type='submit' value='Commander' id='go' />";
		echo "</form>";
/*	
	//Le premier formulaire
	if ($dispo) {
		echo "<form action='ExecuteCommande.php' method='POST'>";
		echo "<fieldset>";
		for ($k=0;$k<$_POST["NBR"];$k++){
			$nom1="NumFilm".$k;
			$nom1=$_POST["$nom1"];
			$nom2="Support".$k;
			$nom2=$_POST["$nom2"];
			$nom3="Ex".$k;
			$i=0;
			$d_f=get_res("select Statut from CASSETTES where NoFilm=$nom1 and Support='".$nom2."'",$link);
			if ($d_f=="disponible") {
				$i++;
				$n_f=get_res("select NoFilm from FILMS where NoFilm=$nom1",$link);
				echo "<input type='checkbox' name='NumFilm'.$i value=$n_f checked='checked' />$n_f";
//				echo "colonne Statut de CASSETTES mise a 'reservee' <br />";
//				"update CASSETTES set Statut='reservee' where NoFilm=$_POST['$nom1'] and Support='$_POST['$nom2']'";
				$n_e=get_res("select NoExemplaire from CASSETTES where NoFilm=$nom1 and Support='$nom2'",$link);
//				echo "ajouter d'1 ligne dans EMPRES <br />";
//				"INSERT INTO EMPRES VALUES($n_f,$n_e,$_POST['PASS'],NOW())";
//				echo "le numéro d'examplaire est transmis par 1 champ caché nommé Exi <br />";
				echo "<input type='hidden' name='$nom3' value='$n_e' />";
			}
		}
		$code=$_POST["PASS"];
		//le numéro d'abonné est transmis par 1 champ caché nommé PASS
		echo "<input type='hidden' name='PASS' value='$code' />";
		//le nombre de case cochée est transmis par 1 champ caché nommé NBR
		echo "<input type='hidden' name='NBR' value='$i' />";
		//bouton "Commander"
		echo "<input type='submit' value='Commander' />";
		echo "</fieldset>";
		echo "</form>";
	}
	else
		echo "Aucun film demandé n'est pas disponible. "*/
	mysql_close($link);
?>
	<!-- Le second formulaire-->
		<CENTER>
		<form action="Commande.php" method="POST">
			<fieldset>
				<label>Vous voulez revoir le choix ?</label><br />
<?php
				echo "<input type='hidden' name='nomC_IdC' value='$nom_t' />";
				echo "<input type='hidden' name='numA_IdC' value='$pass_t' />";
?>
				<input type="submit" value="Revoir le choix" />
			</fieldset>
		</form>
		</CENTER>
<script type="text/javascript">
function le_cookie(nom) {
	var arrStr = document.cookie.split("; ");
	for(var i = 0;i < arrStr.length;i ++){
		var temp = arrStr[i].split("=");
		if(temp[0] == nom) return unescape(temp[1]);
	} 
}

var nbr_init=le_cookie("nbr");

function to_check(le) {
	if (le.checked) nbr_init++;
	else nbr_init--;
	if (nbr_init==0) document.getElementById("go").style.display="none";
	else document.getElementById("go").style.display="block";
	var nbr_final="nbr_final="+nbr_init;
	document.cookie=nbr_final;
}
</script>
	</body>
</html>
