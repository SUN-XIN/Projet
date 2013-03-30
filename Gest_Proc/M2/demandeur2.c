#include "../include/demandeur.h"

//#define M1 "127.0.0.1"
//#define M1 "192.168.0.11"
#define M1 ""
//#define M1 ""
#define M2 ""

int main(){
	
	init();
	int u1,u2;
	int s;
	
	u1=rexecute(M1,"ls");
	sleep(1);
	
	u1=rexecute(M2,"../test/test2");
	sleep(1);
	
	u1=rexecute(M2,"../test/test");
	sleep(1);


	/*
	affichage();
	sleep(5);
	/*
	if (!rkill(u1,SIGUSR1)) {printf("Erreur\n");}
	
	sleep(1);
	*/
	/*
	u2=rexecute(M1,"./test/test");
	//u2=rexecute("127.0.0.1","ls");
	printf("MAIN : UID recu %d\n",u2);

	*/
	rwait(&s);
	
	rwait(&s);
	
	rwait(&s);
	
	printf("MAIN : FIN DE PROG\n");
	
	return EXIT_SUCCESS;
}
