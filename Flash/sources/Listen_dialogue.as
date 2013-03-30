package
{
	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import flash.media.*;	
	import fl.controls.Button;
    import fl.controls.Label;
    import fl.controls.RadioButton;
    import fl.controls.RadioButtonGroup;
    import flash.text.*;
//****************************************************************************************
    
	//This part is for the listening for dialogue2
	
	public class Listen_dialogue extends Sprite
    {
		// The declaration of variable used
		private var conteneur:MovieClip;

		private var int_x:uint = 0;
		private var int_y:uint = 0;
		
		var select1:Boolean;
		var select2:Boolean;
		//The declaration of variable for the form of question/answer
		var answer_form:TextFormat = new TextFormat();
		var question_form:TextFormat = new TextFormat();
		//The declaration of variable for the content of question/answer
		var question_t1:TextField = new TextField();
		var question_t2:TextField = new TextField();
		var answer_t:TextField = new TextField();
		//The declaration of variable for the button export
		var b_next:btn_next = new btn_next();			//button for "go to next page"
		var b_correct:btn_correct = new btn_correct();  //button for "correct your choices"
		var b_pause:btn_pause = new btn_pause();        //button for "pause of listening dialogue"
		var b_play:btn_play = new btn_play();			//button for "play of listening dialogue"
		var b_stop:btn_stop = new btn_stop();			//button for ""stop listening dialogue"
		//The declaration of variable for the sign when your choice is right/wrong
		private var r_s:right_s;
		private var r_c:right_c;
		private var w_:wrong_;
//****************************************************************************************
		public function Listen_dialogue() {
			
			//The part of son
			
			var _sound:Sound=new Sound();
			var _channel:SoundChannel=new SoundChannel();
			var url:String="../Listen2/discussion.mp3";
			var _request:URLRequest = new URLRequest(url);
			_sound.load(_request);

			var position:int;				//Note the position of pause,so that continue to replay
			b_pause.visible = false;		//The button play/pause with the same coordinate
			b_play.visible = true;		    //the visible is either "play" or "pause"
			var mark_begin:Boolean = true;  //The mark to judge if this is the beginning of listen
			
			b_pause.addEventListener(MouseEvent.CLICK,listen_pause);
			function listen_pause(e:MouseEvent):void { //the function for the button of pause
				b_pause.visible=false;
				b_play.visible=true;
				position=_channel.position;
				_channel.stop();
			}

			b_play.addEventListener(MouseEvent.CLICK,listen_play);
			function listen_play(e:MouseEvent):void { //the function for the button of play
				b_pause.visible=true;
				b_play.visible=false;
				if (mark_begin){
					mark_begin=false;
					_channel=_sound.play();
				}
				else
 					_channel=_sound.play(position);
			}

			b_stop.addEventListener(MouseEvent.CLICK,listen_stop);
			function listen_stop(e:MouseEvent):void { //the function for the button of stop
				b_pause.visible=false;
				b_play.visible=true;
				position=0;
				_channel.stop();
			}
//****************************************************************************************
			conteneur = new MovieClip();
			this.addChild(conteneur);
			
			//The part of definition of answer/question
			//form: size and color       text: coordinate, size, content
			
			answer_form.size = 18;
			answer_form.color = 0x00ff00;
			
			question_form.size = 22;
			question_form.color = 0x00ff00;
			
			//question1
			question_t1.text = "1.The two young men are talking about _______.";
			question_t1.x = 192;
			question_t1.y = 55;
			question_t1.width = 600;
			question_t1.height = 30;
			question_t1.setTextFormat(question_form);
			conteneur.addChild(question_t1);
			//question2
			question_t2.text = "2.Harry thinks the man talked a lot of rubbish because Harry believes _________. ";
			question_t2.x = 192;
			question_t2.y = 152;
			question_t2.width = 600;
			question_t2.height = 55;
			question_t2.wordWrap = true;
			question_t2.setTextFormat(question_form);
			conteneur.addChild(question_t2);
//****************************************************************************************
            
			//The part of definition of button(coordinate and show in stage)
			
			b_next.x = 716;
			b_next.y = 362;
		    conteneur.addChild(b_next);
			
			b_correct.x = 600;
			b_correct.y = 362;
			conteneur.addChild(b_correct);
			
			b_play.x = 65;
			b_play.y = 88;
			conteneur.addChild(b_play);
			
			b_pause.x = 65;
			b_pause.y = 88;
			conteneur.addChild(b_pause);
			
			b_stop.x = 130;
			b_stop.y = 88;
			conteneur.addChild(b_stop);
			
			b_next.enabled = false;  //We can click the button "next" only when you'd chosen
			b_correct.enabled = false;
//****************************************************************************************
	        
			//The part of RadioButton
			//There're 2 group of RadioButton for each question
			
			//Add the text(answers) to each group
			var question1:RadioButtonGroup = new RadioButtonGroup("question1");
			int_x = 240; //All the 3 option3 with the same coordinate:y but not x
    	    createAnswer1("a film",question1);     //option1 for question1
        	createAnswer1("a lecture",question1);  //option2 for question1
       	    createAnswer1("a book",question1);     //option3 for question1

        	var question2:RadioButtonGroup = new RadioButtonGroup("question2");
			int_y = 220;
        	createAnswer2("new kinds of machinery will be invented",question2);
        	createAnswer2("atomic power will be the only sort of power",question2);
        	createAnswer2("coal will become important again",question2);
			
        	function createAnswer1(answer_text:String, question:RadioButtonGroup):void{
				var answer:RadioButton = new RadioButton();
				answer.group = question;
          		answer.label = answer_text;
				answer.setStyle("textFormat",answer_form);
				answer.width = 150;
          		answer.move(int_x,100);  //All the 3 option3 with the same coordinate:y but not x
		        int_x = int_x + 180;     //The space between chaque 2 option is 180
      		    conteneur.addChild(answer);
				answer.addEventListener(Event.CHANGE,correction);
        	}

        	function createAnswer2(answer_text:String, question:RadioButtonGroup):void{
        		var answer:RadioButton = new RadioButton();
        		answer.group = question;
        		answer.label = answer_text;
				answer.setStyle("textFormat",answer_form);
				answer.width = 600;
        	    answer.move(240,int_y);
        		int_y = int_y + 38;
        		conteneur.addChild(answer);
				//The Listener for your select choice or change choice
				answer.addEventListener(Event.CHANGE,correction);
       		 }
//**************************************************************************************** 
			  
			  //The part of correction
			  
			 function correction(e:Event):void{
				 if ((question1.selection == null) || (question2.selection == null))
				 	return;
				 b_correct.enabled = true;
           		 var choice1:RadioButton = question1.selection;
				 var choice2:RadioButton = question2.selection;
				 //The right answers for question1,2
				 if (choice1.label == "a lecture") 
					 select1 = true;
				 if (choice2.label == "atomic power will be the only sort of power")
					 select2 = true;
			 }
			 //The listener for the button "correct". when you click the button, show the result
			 b_correct.addEventListener(MouseEvent.MOUSE_DOWN,the_result);
			 
			 function the_result(e:Event):void{
				 b_correct.enabled = false;
				 b_next.enabled = true;
				 for (var i=0;i<3;i++) {
				 	question1.getRadioButtonAt(i).mouseEnabled = false;
				 	question2.getRadioButtonAt(i).mouseEnabled = false;
				 }
				 //The mark for the right/wrong answer
				 if (select1) {
					 r_s = new right_s();
					 r_s.x = question1.selection.x - 15;
					 r_s.y = question1.selection.y + 32;
					 conteneur.addChild(r_s);
				 }
				 else {
					 w_ = new wrong_();
					 w_.x = question1.selection.x -15;
					 w_.y = question1.selection.y +32;
					 conteneur.addChild(w_);
				 }
				 if (select2) {
					 r_s = new right_s();
					 r_s.x = question2.selection.x - 15;
					 r_s.y = question2.selection.y + 32;
					 conteneur.addChild(r_s);
				 }
				 else {
					 w_ = new wrong_();
					 w_.x = question2.selection.x - 15;
					 w_.y = question2.selection.y + 32;
					 conteneur.addChild(w_);
				 }
				 
			 }
			 
			function viderConteneur(){
				if (conteneur.numChildren>0)
					for (var i:uint = 0; i < 15; ++i)
						conteneur.removeChildAt(0);
			}
			//When you finish this dialogue and click button "next"
			//clean up all in the stage and begin the next dialogue
			b_next.addEventListener(MouseEvent.MOUSE_DOWN,go_next);
			function go_next(e:Event):void {
				viderConteneur();
				_channel.stop();
				load_swf("Listen_dialogue2.swf");
			}
			
			function load_swf(name_swf:String):void{
				var charge:Loader = new Loader();
				charge.load(new URLRequest(name_swf));
				addChild(charge);
			}
			
			

		}//public function Listen_dialogue()
	}//public class Listen_dialogue extends Sprite
}