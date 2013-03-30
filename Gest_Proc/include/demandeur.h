#include "strcut_donnee.h"

int sockE,sockR; 		//sockE - socket pour echanger msg avec ecouteur
						//sockR - socket pour rps
						
struct sockaddr_in dest,recu,loc,mul,mulr;
pMach teteM=NULL; 
int nbrM=0;

void ajouterProcD(char *add,int theUID,int theFINI,int theETAT) {
	pProc pp=NULL;;
	pMach pm=teteM;
	if (!pm) { //tete de liste machine est vide=>la premiere fois ajouter
		printf("Tete vide -> 1er machine ajoutée dans la liste\n");
		nbrM=1;
		teteM=(pMach)(malloc(sizeof(mach)));
		strcpy(teteM->addr,add);
		teteM->prec=NULL;
		teteM->suiv=NULL;
		if (theUID>0) {
			teteM->nbrProc=1;
			pp=(pProc)(malloc(sizeof(proc)));
			pp->uid=theUID;
			pp->fini=theFINI;
			pp->etat=theETAT;
			pp->suiv=NULL;
			pp->prec=NULL;
			pp->m=teteM;
			teteM->tp=pp;
		}
		else {
			teteM->nbrProc=0;
			teteM->tp=NULL;
		}
	}
	else { //parcourir la liste machine à chercher add
		printf("Ajoute dans la liste");
		int fin=0;
		while (strcmp(pm->addr,add)!=0) {
			if (pm->suiv) pm=pm->suiv;
			else {fin=1;break;}
		}
		if (fin) { //ne trouve pas=>ajouter 1 nouvelle manchine dans la liste
			nbrM++;
			printf(",ne trouver pas!\n");
			pm->suiv=(pMach)(malloc(sizeof(mach)));
			strcpy(pm->suiv->addr,add);
			pm->suiv->prec=pm;
			pm->suiv->suiv=NULL;
			if (theUID>0) {
				pm->suiv->nbrProc=1;
				pm->suiv->tp=(pProc)(malloc(sizeof(proc)));
				pm->suiv->tp->uid=theUID;
				pm->suiv->tp->fini=theFINI;
				pm->suiv->tp->etat=theETAT;
				pm->suiv->tp->suiv=NULL;
				pm->suiv->tp->prec=NULL;
				pm->suiv->tp->m=pm->suiv;
			}
			else  {
				pm->suiv->nbrProc=0;
				pm->suiv->tp=NULL;
			}
		}
		else { //la machine demandant exite
			printf(",trouver!\n");
			if (theUID) {
				pm->nbrProc++;
				if (!pm->tp) { //liste de processus est vide=>le premier proc de cette machine
					pm->tp=(pProc)(malloc(sizeof(proc)));
					pm->tp->uid=theUID;
					pm->tp->fini=theFINI;
					pm->tp->etat=theETAT;
					pm->tp->prec=NULL;
					pm->tp->suiv=NULL;
					pm->tp->m=pm;
				}
				else {
					pp=pm->tp;
					while (pp->suiv) {pp=pp->suiv;}
					pp->suiv=(pProc)(malloc(sizeof(proc)));
					pp->suiv->uid=theUID;
					pp->suiv->fini=theFINI;
					pp->suiv->etat=theETAT;
					pp->suiv->prec=pp;
					pp->suiv->suiv=NULL;
					pp->suiv->m=pm;
				}
			}
			else { //cas particulier : une machine eteind, puis redemarre
				//supprimer tous les proc de add
				printf("supprimer tous les proc de add\n");
			}
		}
	}
}

void sighand(int sig){
	//envoyer vers ecouteurs -> je suis en vie
	envoyer(9,"",sockE,mul);
}

void init(){
	/* init signal */
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGUSR1);
	sigdelset(&ens,SIGINT);
	sigprocmask(SIG_SETMASK,&ens,NULL);
		
	struct sigaction act;
	act.sa_handler=sighand;
	act.sa_flags=SA_RESTART;
	sigaction(SIGUSR1,&act,0);
	/*********************************** init pour echanger msg avec ecouteur **************************************/
	/* creation sockE - socket pour echanger msg avec ecouteur */
	/* Creating UDP socket */
	if ((sockE = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Erreur demandeur init : socket\n");
		exit(1);
	}
	/* remplir le nom local */
	memset((char *)&loc,0,sizeof(loc));
	loc.sin_family=AF_INET;
	loc.sin_port=htons(PortD);
	loc.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockE, (struct sockaddr *)&loc, sizeof(struct sockaddr)) == -1) {
		perror("Erreur demandeur init rps : bind\n");exit(1);}
	/**************************participer la groupe mulcast pour rps***********************************/
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr=inet_addr(mulAddrRPS);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if(setsockopt(sockE,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) <0) {
		perror( "Erreur demandeur init : setsockopt\n");exit(1);}
	/*********************************** compter nbr ecouteur sur reseau **************************************/
	/* remplir le nom de mul */
	memset((char *)&mul,0,sizeof(mul));
	mul.sin_family=AF_INET;
	mul.sin_port=htons(PortE);
	mul.sin_addr.s_addr = inet_addr(mulAddr);
	//envoyer 1 msg vers mulcast
	envoyer(8,"",sockE,mul);
	printf("compter nbr ecouteur...\n");
	//attendre les reponses des ecouteurs pendant 3s
	fd_set fd;
	struct timeval timeout;
	timeout.tv_sec=3;
	timeout.tv_usec=0;
	FD_ZERO(&fd); 
	FD_SET(sockE,&fd);
	int s;
	msg msgR;
	while(1) {
	   s = select(sockE+1, &fd, NULL, NULL, &timeout);
	   if (s>0) {
		   recvfrom(sockE,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len);
		   if (msgR.type==88) {
				printf("Recu 1 reponse d'1 ecouteur '%s', ",inet_ntoa(recu.sin_addr));
				ajouterProcD(inet_ntoa(recu.sin_addr),-1,-1,-1);}
	   }else break;
	   sleep(1); 
	}
	printf("il y a %d au total\n",nbrM);
	//chaque 5s envoyer vers tous ecouteurs : je suis en vie
	set_ticker(5);
	
	printf("Fin init\n");
}

msg testMSG(msg msgR,int val){
	pProc pp=NULL;
	if (msgR.type==val) return msgR;
	msg mR=msgR;
	while (mR.type!= val) {
		printf("msg en attente...\n");
		switch (mR.type) {
			case 33 : //reponse pour rwait
				printf("\nRecu 1 reponse rwait\n");
				/* changer l'info du processus */
				/* data "UID;status" */
				char buff[12],theU[10],theS[2];
				strcpy(buff,msgR.data);
				int t=0,i;
				while (buff[t]!=';') {
					theU[t]=buff[t];
					t++;}
				t++;
				for (i=t;i<strlen(buff);i++) {theS[i-t]=buff[i];}
				pp=chercherParUID(atoi(theU),teteM);
				pp->fini=OUI;
				pp->etat=atoi(theS);
				/* Envoyer la reponse */
				/*
				msgE.type=333;
				strcpy(msgE.data,"");
				sendto(sockE,&msgE,sizeof(msgE),0,(struct sockaddr *)&recu,len);*/
				break;
			case 88 : //msg de reponse pour compter nbr ecouteur
				printf("\nRecu 1 reponse d'1 nouveau ecouteur, ");
				ajouterProcD(inet_ntoa(recu.sin_addr),-1,-1,-1);
				printf("il y a %d au total\n",nbrM);
				break;
			case 7 : //msg de rps
				printf("\nRecu 1 msg de rps\n");
				pMach pm=teteM;
				char uids[MAXBUFF],unUid[10];
				strcpy(uids,"");strcpy(unUid,"");
				while (pm) {
					if (pm->nbrProc<1) {pm=pm->suiv;continue;}
					pp=pm->tp;
					while (pp) {
						sprintf(unUid,"%d",pp->uid);
						strcat(uids,unUid);
						strcat(uids,";");
						pp=pp->suiv;}
					pm=pm->suiv;}
				uids[strlen(uids)-1]='F';
				printf("data = %s\n",uids);
				/* Envoyer la reponse */
				envoyer(77,uids,sockE,recu);
				break;
			default : 
				printf("\n\nmsg type %d \n",msgR.type);
				break;
		}
		fflush(stdout);
		if ((recvfrom(sockE,&mR,sizeof(mR),0,(struct sockaddr *)&recu,&len))==-1) {printf("Erreur demandeur testMSG : recvfrom() testMSG\n");exit(1);}
	}
	return mR;
}

t_uid rexecute(char *add,char *code){
	sigLock();
	printf("\nCommence rexecute\n");
	if (strlen(add)<7) { // equilibre charge machine
		printf("%s\n",teteM->addr);
		if (!teteM) {printf("Aucune machine disponible!\n");exit(1);}
		char theAddr[15];
		pMach pm=teteM;
		int minProc=99;
		while (pm) {
			if (pm->nbrProc<minProc) {
				strcpy(theAddr,pm->addr);
				minProc=pm->nbrProc;}
			pm=pm->suiv;}
		add=theAddr;}
	else {
		pMach pm=teteM;
		int trouver=0;
		while(pm) {
			if (strcmp(pm->addr,add)==0) {trouver=1;break;}
			pm=pm->suiv;}
		if (!trouver) {printf("La machine n'est pas disponible\n");exit(1);}}
	//test @
	in_addr_t addr;
	if ((int)(addr=inet_addr(add))==-1) {perror("Erreur demandeur rexecute : IP-address must be of the form a.b.c.d\n");exit(1);}
	/* remplir le nom de dest */
	memset((char *)&dest,0,sizeof(dest));
	dest.sin_family=AF_INET;
	dest.sin_port=htons(PortE);
	dest.sin_addr.s_addr = addr;
	//envoyer msg1 vers add
	envoyer(1,code,sockE,dest);
	printf("Envoyer le demande rexecute\n");
	//attendre la reponse
	printf("Attendre la reponse\n");
	msg msgR;
	if ((recvfrom(sockE,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len))==-1) {printf("Erreur demandeur rexecute : recvfrom()\n");exit(1);}
	testMSG(msgR,11);
	printf("recu UID %d\n",atoi(msgR.data));
	ajouterProcD(add,atoi(msgR.data),NON,-1); //ajouter ce proc dans les listes
	printf("fin de rexecute\n");
	sigUnlock();
	affichage(teteM);
	return atoi(msgR.data);
}

int rkill(t_uid u,int sig) {
	sigLock();
	printf("\nCommence rkill\n");
	printf("Demande envoyer sig %d vers proc %d\n",sig,u);
	//chercher l'@ de t_uid
	pProc pp=chercherParUID(u,teteM);
	if (pp) printf("trouve\n");
	else {sigUnlock();return 0;} //UID unconnu
	in_addr_t addr=inet_addr(pp->m->addr);
	/* remplir le nom de dest */
	memset((char *)&dest,0,sizeof(dest));
	dest.sin_family=AF_INET;
	dest.sin_port=htons(PortE);
	dest.sin_addr.s_addr = addr;
	//envoyer msg2 vers add
	char theS[10],theData[MAXDATA];
	sprintf(theS,"%d",sig);
	sprintf(theData,"%d",u);
	strcat(theData,";");
	strcat(theData,theS);
	envoyer(2,theData,sockE,recu);
	printf("Envoyer le demande rkill vers %d\n",u);
	//attendre la reponse
	printf("Attendre la reponse\n");
	msg msgR;
	if ((recvfrom(sockE,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len))==-1) {sigUnlock();return 0;}
	testMSG(msgR,22);
	printf("Recu la reponse, fin de rkill\n");
	sigUnlock();
	affichage(teteM);
	if (atoi(msgR.data)) return 1;//cas erreur
	return 0;
}

int rwait(int *status) {
	sigLock();
	printf("\nCommence rwait\n");
	in_addr_t addr;
	pProc pp=chercherParFINI("",teteM);
	/* verrifier si la liste contient 1 proc fini*/	
	if (pp) {
		printf("trouver 1 proc fini\n");
		*status=pp->etat;
		int theU=pp->uid;
		supprimer(pp);
		sigUnlock();
		return theU;}
	printf("aucun proc fini, wait...\n");
	pMach pm=teteM;
	while(pm) { //ne trouver aucun proc fini,envoyer la demande "rwait"
		if ((pm->nbrProc)<1) {pm=pm->suiv;continue;} //la machine n'a pas mon proc
		addr=inet_addr(pm->addr);
		/* remplir le nom de dest */
		memset((char *)&dest,0,sizeof(dest));
		dest.sin_family=AF_INET;
		dest.sin_port=htons(PortE);
		dest.sin_addr.s_addr = addr;
		//envoyer msg3 vers add
		envoyer(3,"",sockE,dest);
		printf("Envoyer le demande rwait vers %s\n",pm->addr);
		pm=pm->suiv;
	}
	msg msgR;
	if ((recvfrom(sockE,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len))==-1) {printf("Erreur demandeur rwait : recvfrom() rwait\n");exit(1);}
	msgR=testMSG(msgR,33);
	/*
	//envoyer msg3 vers add
	msgE.type=333;
	strcpy(msgE.data,"");
	sendto(sockE,&msgE,sizeof(msgE),0,(struct sockaddr *)&recu,len);*/
	printf("data %s\n",msgR.data);
	//msgR.data contien "UID;status"
	char buff[12],theU[10],theS[2];
	memset(buff,0,sizeof(buff));
	memset(theU,0,sizeof(theU));
	memset(theS,0,sizeof(theS));
	strcpy(buff,msgR.data);
	int t=0,i;
	while (buff[t]!=';') {theU[t]=buff[t];t++;}
	t++;
	for (i=t;i<strlen(buff);i++) {theS[i-t]=buff[i];}
	*status=atoi(theS);
	printf("supprimer proc %d\n",atoi(theU));
	supprimer(chercherParUID(atoi(theU),teteM));
	printf("Fin de rwait\n");
	sigUnlock();
	affichage(teteM);
	return atoi(theU);
}

