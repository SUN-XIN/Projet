package {

	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import flash.media.*;	
	import fl.controls.Button;
    import fl.controls.Label;
    import fl.controls.RadioButton;
    import fl.controls.RadioButtonGroup;
    import flash.text.*;

	import flash.text.TextField;
	import flash.text.TextFormat;

	public class QCM extends MovieClip {
		
		//Conteneur contain the RadioButton 
		private var conteneur:MovieClip;
		
		//List of question
		private var arrayQuestion:Array = new Array();
		//List of option
		private var arrayReponse:Array = new Array();
		//List of answer
		private var arrayBonneRep:Array = new Array();
		//This list stock only one menber which is your choice
		private var arrayQuestionsPosee:Array = new Array();
		//Group of radio bouton
		private var rbg:RadioButtonGroup;
		//*****************************************************************************************
		//part of import
		private var repJuste:juste;         //mark for right answer
    	private var repRight_c:right_c      //fun_image for right answer
		private var repFaux:faux;		    //mark for wrong answer
		private var r_n;result_negatif;  	//picture if you have a bad result at the end
		private var r_p;result_positif;     //picture if you have a good result at the end
		private var r_m;result_moy;			//picture when you have a general result at the end
		//*****************************************************************************************
		private var resultat:String;

		//score current
		var score:int = 0;
		//indice of the question current in the list of question
		var currentQuestion:int = 0;
		//the number of question current
		var numQuestion:int = 0;
		//indice of question current in the list of arrayQuestionPosee
		var	currentQuestionPosee:int = 0;
		
		var xmlDoc:XML;
		
		//name of font 
		var font:Font = new fff();
		//declaration of the form for option/question
		var my_fmt:TextFormat = new TextFormat();
		var scForm:TextFormat = new TextFormat();
		var qForm:TextFormat = new TextFormat();
		
		//declaration of text for question/score
		var question:TextField = new TextField();
		var tScore:TextField = new TextField();
		
		//button "next"/"correct"
		var bou_suiv:btn_suivant = new btn_suivant();
		var bou_verif:btn_verif = new btn_verif();
		
		//mark for the chronometry of QCM
		var eval:int;
		
		//variable share all the applications(cookie)
		var share:SharedObject = SharedObject.getLocal("monCookie", "/");
		//*****************************************************************************************
		public function QCM(e:int) {
			
			eval = e;
			
			conteneur = new MovieClip();
			this.addChild(conteneur);
		
			// charge qcm.xml contains the list of question
			var chargeur:URLLoader = new URLLoader();
			var adresse:URLRequest=new URLRequest("../xml/qcm.xml");

			chargeur.addEventListener(Event.COMPLETE, finDuChargement);
			chargeur.addEventListener(IOErrorEvent.IO_ERROR, indiquerErreur);
			chargeur.load(adresse);
			
			//definition of the form for question(font, size and color)
			my_fmt.font = font.fontName;
			my_fmt.size = 22;
     	    my_fmt.color = 0x00FF00;
			//definition of the form for score(font, size and color)
			scForm.font = font.fontName;
			scForm.size = 20;
     	    scForm.color = 0x00FF00;
			//definition of the form for option(font, size and color)
			qForm.font = font.fontName;
			qForm.size = 20;
     	    qForm.color = 0x00FF00;
			//*****************************************************************************************
			//definition of the text for question(content, coordinate, form etc)
			question.text = "";
			question.selectable = false;
			question.embedFonts = true;
			question.wordWrap = true;
			question.x = 80;
			question.y = 90;
			question.width = 700;
			question.height = 55;
			question.setTextFormat(my_fmt);  
			question.multiline = true;
			this.addChild(question);
			//definition of the text for score(content, coordinate, form etc)
			tScore.x = 700;
			tScore.y = 60;
			tScore.selectable = false;
			tScore.embedFonts = true;
			tScore.multiline = false;
			tScore.htmlText = "0/10";
			tScore.setTextFormat(scForm);
			this.addChild(tScore);
			//*****************************************************************************************
			//definition of the button "next" and diaplay 
			bou_suiv.x = 700;
			bou_suiv.y = 360;
			this.addChild(bou_suiv);
			//definition of the button "correct" and display
			bou_verif.x = 570;
			bou_verif.y = 350;
			this.addChild(bou_verif);
			//the Listener for button "next"/"correct"
			bou_suiv.addEventListener(MouseEvent.MOUSE_DOWN, questionSuivante);
			bou_verif.addEventListener(MouseEvent.MOUSE_DOWN, verif);
		}
		//*****************************************************************************************
		// function for charge the xml
		//build the list of question/option
		function finDuChargement( event:Event ) {

			xmlDoc=new XML(event.target.data);
			//cycle of question
			for (var i = 0; i<xmlDoc.children().length(); ++i) { 
				arrayQuestionsPosee.push(i);
				arrayQuestion.push(xmlDoc.question[i].attribute("texte"));
				arrayReponse.push(new Array());
				//cycle of option for each question
				for (var j = 0; j<xmlDoc.question[i].children().length(); j++) {  
					arrayReponse[arrayReponse.length - 1].push(xmlDoc.question[i].reponse[j]);
					//note the number in the liste of right answer
					if (xmlDoc.question[i].reponse[j].attribute("bonne") == "vrai") {
						arrayBonneRep.push(xmlDoc.question[i].reponse[j]);
					}
				}
			}
			//we can click the button "next" only when we've a choice
			bou_suiv.enabled = false;
			bou_verif.enabled = true;
			//initiation of QCM
			init();
		}

		//function error and display the error info
		function indiquerErreur( event:Event ) {
			trace(event);
		}
		
		//initiation of QCM
		function init():void {
			//espace between each 2 radio boutons(option)
			var espace:int = 0;
			
			currentQuestionPosee = Math.round(Math.random() * (arrayQuestionsPosee.length - 1) + 1)
			currentQuestion = arrayQuestionsPosee[currentQuestionPosee];
			//change the text of question
			question.htmlText = "Question " + (numQuestion+1) + " : " + arrayQuestion[currentQuestion];
			question.setTextFormat(my_fmt);
			rbg = new RadioButtonGroup("question");
			//build the radio buttons with the text of option
			for(var i=0;i<4;++i){
				var rb:RadioButton = new RadioButton();
				rb.group = rbg;
				rb.label = arrayReponse[currentQuestion][i];
				rb.width = 500; 
				rb.setStyle("textFormat",qForm);
				//4 options for each question. The coordinate.x of option are the same, but not y
				rb.move(250,160+espace);			
				espace += 40;
				conteneur.addChild(rb);
			}
		}
		
		//functiont when you click the button "next"
		function questionSuivante(e:MouseEvent):void {
			//if you've had a choice
			if (!bou_suiv.enabled)
				return;
			//if you've done all the 10 question
			if (numQuestion < 10) {
				//you've had a choice -> validate the button "next"
				bou_suiv.enabled = false;
				bou_verif.enabled = true;
				//if you made a right choice
				if (resultat == "juste") {
					this.removeChild(repJuste);
         		    this.removeChild(repRight_c);
				}
				else if (resultat == "faux"){
					this.removeChild(repJuste);
					this.removeChild(repFaux);
				}
				//clear up all the content
				viderConteneur();
				var espace:int = 0;
				currentQuestionPosee = Math.round(Math.random() * (arrayQuestionsPosee.length - 1) + 1)
				currentQuestion = arrayQuestionsPosee[currentQuestionPosee];
				//change the text of question
				question.htmlText = "Question " + (numQuestion+1) + " : " + arrayQuestion[currentQuestion];
				question.setTextFormat(my_fmt);
				rbg = new RadioButtonGroup("question");
				//build the radio buttons with the text of option
				for(var i=0;i<4;++i){
					var rb:RadioButton = new RadioButton();
					rb.group = rbg;
					rb.label = arrayReponse[currentQuestion][i];
					rb.width = 500;
					rb.setStyle("textFormat",qForm);
					rb.move(250,160+espace);
					espace += 40;
					conteneur.addChild(rb);
				}
			}
			//when you've finished all the 10 question
			else {
				viderConteneur();
				if (resultat == "juste") {
					this.removeChild(repJuste);
					this.removeChild(repRight_c);
				}
				else if (resultat == "faux"){
					this.removeChild(repJuste);
					this.removeChild(repFaux);
				}
				//give out your totle score
				question.htmlText = "Let's look at your final score : " + score + "/10";	
				question.setTextFormat(my_fmt);
					
				this.removeChild(bou_verif);
				this.removeChild(bou_suiv);
				//the level of your result 0-4:bad_result
				//						   5-6;general_result
				//						   7-10;goog_result
				//and play the corresponding sons
				if (score<4) {
					r_n = new result_negatif();
					r_n.x=400;
					r_n.y=240;
         			 show_son("../son/bad_result.mp3");
					this.addChild(r_n);
				}
				else{
					if (score<7) {
						r_m = new result_moy();
						this.addChild(r_m);
						r_m.x=400;
						r_m.y=240;
            			show_son("../son/moy_result.mp3");
					}
					else {
						r_p = new result_positif();
						this.addChild(r_p);
						r_p.x=400;
						r_p.y=240;
           			    show_son("../son/good_result.mp3");
					}
				}
				
			}
		}
		
		// Function when you click the button "correct"
		function verif(e:MouseEvent):void {
			//if you've had a choice
			if (!bou_verif.enabled)
				return;
			if (rbg.selection == null) {
                return;
            }
			else{
				var j:int = -1;
				//you've had a choice -> validate the button "next"
				bou_suiv.enabled = true;
				bou_verif.enabled = false;
				//the correction of the choice
				for(var i=0;i<rbg.numRadioButtons;i++){
					rbg.getRadioButtonAt(i).mouseEnabled = false;
					if(rbg.getRadioButtonAt(i).label == arrayBonneRep[currentQuestion]){
						j = i;
					}
				}
				//compare your choice and the right answer 
				resultat = (rbg.selection.label == arrayBonneRep[currentQuestion]) ? "juste" : "faux";
				   if(resultat == "juste"){	
				   		//score +1 when you've had a right choice
				 		score++;
						if(e)
							share.data.score2 = score;
						//display your new score and the corresponding fun_image 
						tScore.htmlText = score + "/10";
						tScore.setTextFormat(scForm);
					    repJuste = new juste();
             		    repRight_c =  new right_c();
					    repJuste.x = rbg.selection.x - 30;
					    repJuste.y = rbg.selection.y + 32 ;

					    repRight_c.x = rbg.selection.x - 30;
					    repRight_c.y = rbg.selection.y + 32 ;
					    this.addChild(repJuste);
            		    this.addChild(repRight_c);
				   }
				   else{
					   //you've had the wrong choice
					   repFaux = new faux();
					   repFaux.x = rbg.selection.x - 30;
					   repFaux.y = rbg.selection.y + 32;
					   this.addChild(repFaux);
					   repJuste = new juste();
					   repJuste.x = rbg.getRadioButtonAt(j).x - 30;
					   repJuste.y = rbg.getRadioButtonAt(j).y + 32;
					   this.addChild(repJuste);
				   }
		       } 
			   arrayQuestionsPosee.splice(currentQuestionPosee, 1);
			   numQuestion++;
		}
		
		//function to clear up the content
		function viderConteneur(){
			if (conteneur.numChildren>0) {
				for (var i:uint = 0; i < 4; ++i) {
					conteneur.removeChildAt(0);
				}
			}
		}
		//function to play the so
		function show_son(mp3:String):void{
			var requete:URLRequest = new URLRequest (mp3);
			var son:Sound = new Sound (requete); // Chargement progressif
			son.play ();
		}


	}// fin classe
}

