<?php
		echo "<form action='#' method='POST'>";
		echo "<fieldset>";
		echo "<input type='checkbox' name='a' value='a1' checked='checked' />1<br />";
		echo "<input type='checkbox' name='b' value='b1' checked='checked' />2<br />";
		echo "<input type='checkbox' name='c' value='a1' />3<br />";
		echo "<input type='hidden' name='NBR' value='nbr' />";

			echo "<input type='submit' value='Commander' />";

		echo "</fieldset>";
		echo "</div>";
		echo "</form>";
?>
<div>
<table>
<tr name="tr1">
<td><input type=checkbox id="1"></td>
<td><input type=checkbox id="1"></td>
</tr>
<tr name=tr2">
<td><input type=checkbox id="2"></td>
<td><input type=checkbox id="2" onclick="to_check(this)"></td>
</tr>
<tr name="tr3">
<td><input type=checkbox id="3"></td>
<td><input type=button value="button" onclick="checkit()"> </td>
</tr>
<table>
</div>
<script>
function checkit()
{
var chk = false;
for(var i=1;i<6;i++)
if(document.getElementById("ch_"+i).checked) chk=true;
if(!chk)alert("no choce");
}

function to_check(le_tr,le) {
	var arr = document.getElementsById("2");
	var num = 0;
	for(var i=0;i<arr.length;i++){
            if(arr[i].checked){
                num++;
            }
        }
	alert("Ne choisissez que 1, SVP"+num);
	le.checked=false;

}
</script>

<span id="num"></span><br/>
  <input name="ckb" type="checkbox" value="1" onclick="sss('ckb',this)" />
  <input name="ckb" type="checkbox" value="2" onclick="sss('ckb',this)"  />
  <input name="ckb" type="checkbox" value="3" onclick="sss('ckb',this)"  />
  <input name="ckb" type="checkbox" value="4" onclick="sss('ckb',this)"  />
<?php
	$n="ckb";
	echo "<input name=$n type='checkbox' value='5' onclick='sss(&quot;$n&quot;,this)'  />";
?>
  <script>
      function sss(nom,le){
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

<!--
<html> 
<head> 
</head> 
<body> 
<form name="f1"> 
  <p><input type="checkbox" name="C1" value="111"></p> 
  <p><input type="checkbox" name="C1" value="222">222</p> 
  <p><input type="checkbox" name="C1" value="333">333</p> 
  <p><input type="button" value="按鈕" name="B1" onClick="getValue();"></p> 
</form> 
</body> 
</html> 
<script> 
function getValue() { 
   var str=""; 
   for (var i=0;i<=document.f1.C1.length-1;i++) 
      if (document.f1.C1[i].checked) 
         str=str+document.f1.C1[i].value+" "; 
   alert(str); 
   }    
</script>

<style type="text/css">
	.informations_cachees{
		visibility:hidden;
	}
	.informations_visibles{
		visibility:visible;
	}
</style>

<span id="informations" class="informations_cachees">
blablabla
blablabla
</span>

<input type="textbox" id="groupe" name="groupe" onclick="valide();" />Groupe
<script>
function valide()
{
	if(document.getElementById("informations").class == "informations_cachees")
		document.getElementById("informations").class = "informations_visibles";
	else
		document.getElementById("informations").class = "informations_cachees";
}
</script>
-->
