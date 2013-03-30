package {
    //import the class Sprite and Loader
    import flash.display.*;
    //import the class MouseEvent
    import flash.events.*;
    //for charge the class URLRequest
    import flash.net.URLRequest;
    //import clip BulleClp
    import BulleClp;

    //Definition of the class Personne
    public class Personne extends Sprite{
    //Definition of the attributes of class
	private var prenom:String;
	private var recommende:String;
	private var photo:Loader;
	private var info:BulleClp;

 	//Definition of the fonction constructor 
 	public function Personne( p:String, n:String) {
	 	setRecommende(n);
 		setPrenom(p);
        setPhoto("../photo/"+p+".jpeg");
	}

 	//Definition of comportment sePresente()
 	public function sePresente():String {
  	   	return recommende;
 	}

    public function sAffiche(tmp:Loader):void {
		addChild(tmp);
		tmp.addEventListener(MouseEvent.MOUSE_OVER, auSurvol);
		tmp.addEventListener(MouseEvent.MOUSE_OUT, alExterieur);

		info = new BulleClp();
        //the clip BulleClp diaplay above the photo 		
        info.x = 2*tmp.width/2;
		info.y = tmp.height/5;
		info.labelOut.text = getRecommende();
      }

		
	private function auSurvol(event:MouseEvent):void {
		addChild(info);
    }
	
	private function alExterieur(event:MouseEvent):void {
		removeChild(info);
	}

 	//function get ： access the info
 	public function getPrenom():String {
		return prenom;
 	}	
	
 	public function getRecommende():String {
		  return recommende;
	}
	
 	//function set : assignment statement
     public function setPhoto(url:String):void {
		 photo= new Loader();
         photo.load(new URLRequest(url));
         photo.contentLoaderInfo.addEventListener(Event.COMPLETE, auComplet);
         photo.load(new URLRequest(url));
     }
	 
     private function auComplet(evt:Event):void {
		 sAffiche(photo);
     }
	 
 	//  Modifier le prénom
 	public function setPrenom(p:String):void {
   		 prenom = p;
 	}
 	// Modifier le nom
 	public function setRecommende(n:String):void {
   		 recommende = n;
	 }
	

   }
}


	
