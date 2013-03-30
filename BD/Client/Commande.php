<?php
	include "Outils.inc";
?>
<html>
	<head>
		<title>Commande</title>
		<style type="text/css">
			#pad{line-height:30px}
		</style>
	</head>
	<body>
<?php
	banniere("IdentificationC",Auteur);
	$link=DB_connect();
//	$_POST['nomC_IdC'] votre nom
//	$_POST['numA_IdC'] numéro d'abonné

	$r_c="";	
	$r_c.=get_res("select Code from ABONNES where Nom='".$_POST["nomC_IdC"]."'",$link);
	if ($r_c!="") {
		if ($_POST["numA_IdC"]==$r_c) {

			echo "<h>Bienvenu ".$_POST["nomC_IdC"]." !</h>";
			$n=get_res("select NbCassettes from ABONNES where Code='".$_POST["numA_IdC"]."'",$link);
			$max=3-$n;
			setcookie("nbr_max",$max);
			$code=$_POST["numA_IdC"];
			$nom=$_POST["nomC_IdC"];
			if ($max==0) echo "Vous pouvez plus commender de films. Le total ne peut pas dépasser 3.";
			else {
				echo "<br />Vous pouvez commender ".$max." cassttes au plus.<br /><br /><br />";
			//Methode 1
/*		
			echo "<form action='ConfirmeCommande.php' method='POST'>";
			echo "<div id='pad'>";
			echo "<fieldset>";
			for ($k=0;$k<$max;$k++) {
				echo "Numéro Film:<input type='text' name='NumFilm'.$k /><br />";
				echo "Support:<input type='radio' name='Support'.$k value='DVD' />DVD<br />";
				echo "Support:<input type='radio' name='Support'.$k value='VHS' />VHS<br />";
			}
			echo "<input type='hidden' name='PASS' value='$code' />";
			echo "<input type='hidden' name='NBR' value='$max' />";
			echo "<input type='submit' value='Commander' />";
			echo "</fieldset>";
			echo "</div>";
			echo "</form>";
*/
			//*****************************
			//Methode 2

			echo "<form action='ConfirmeCommande.php' method='POST'>";
			echo "<fieldset>";
			echo "<table border='1'>";
			echo "<tr><th>Numéro Film</th><th>Support DVD</th><th>Support VHS</th></tr>";
			for ($k=0;$k<$max;$k++) {
				$nom1="NumFilm".$k;
				$nom2="Support".$k;
				echo "<tr>";
				echo "<td><input type='text' name='$nom1' id='$nom1' onchange='cal_nbr()'/></td>";
				echo "<td><input type='checkbox' name='$nom2' value='DVD' onclick='to_check(&quot;$nom2&quot;,this)' /></td>";
				echo "<td><input type='checkbox' name='$nom2' value='VHS' onclick='to_check(&quot;$nom2&quot;,this)' /></td>";
				echo "</tr>";
			}
			echo "<tr style='display:none;'>
			<td><input type='hidden' name='PASS' value='$code' /></td>
			<td><input type='hidden' name='NBR' value='$max' /></td>
			<td><input type='hidden' name='NOM' value='$nom' /></td>
			</tr>";
			echo "</table>";
			echo "<input type='submit' value='Commander' />";
			echo "</fieldset>";
			echo "</form>";

			//*****************************
			}
		}
		else {
			echo "Votre Mot de passe n'est pas correct. <br />";
			echo "<a href='IdentificationC.php' title='Commande de cassettes'>Retourner à la page precedente.</a>";
		}
	}
	else {
		echo "On ne trouve pas votre dossier. Vérifiez-vous votre Nom.<br />";
		echo "<a href='IdentificationD.php' title='Liste des cassette détenues' />Retourner à la page precedente.</a>";
	}
	mysql_close($link);	
?>
<script type="text/javascript">
function le_cookie(nom) {
	var arrStr = document.cookie.split("; ");
	for(var i = 0;i < arrStr.length;i ++){
		var temp = arrStr[i].split("=");
		if(temp[0] == nom) return unescape(temp[1]);
	} 
}

function cal_nbr() {
	var nbr_vide=0;
	var nbr_total=le_cookie("nbr_max");
	for(var i=1;i<nbr_total;i++)
		if (document.getElementById("NumFilm"+i).value=="") nbr_vide++;
	var nbr_final="nbr="+(nbr_total-nbr_vide);
	document.cookie=nbr_final;
}

function to_check(nom,le) {
	var arr = document.getElementsByName(nom);
	var nbr = 0;
        for(var i=0;i<arr.length;i++){
            if(arr[i].checked){
                nbr++;
            }
	}
	if (nbr==2) {
		alert("Ne choisissez que 1, SVP");
		le.checked=false;
	}
}
</script>
	</body>
</html>
