#include "strcut_donnee.h"

int sock; 
struct sockaddr_in dest,recu,loc; 
pMach teteM=NULL;
int nbrM=0;
int ID_MACHINE;

void ajouterProcE(char *add,int theUID,int theFINI,int theETAT) {
	pProc pp;
	pMach pm=teteM;
	if (!teteM) { //tete de liste machine est vide=>la premiere fois ajouter
		printf("Tete vide -> 1er machine ajoutée dans la liste\n");
		nbrM=1;
		teteM=(pMach)(malloc(sizeof(mach)));
		strcpy(teteM->addr,add);
		teteM->enVie=OUI;
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
			teteM->enVie=OUI;
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
			pm->suiv->enVie=OUI;
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
			pm->enVie=OUI;
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
/******************************* Detester si les demandeurs sont en vie *************************************/
void sighand(){ // pour detecter la vie des demandeurs
	printf("\nCommence detecter...\n");
	pMach pm=teteM,pmp,pms;
	while (pm) {
		if (pm->enVie==NON) {
			printf("Supprimer machine %s\n",pm->addr);
			if (!teteM->suiv) {teteM=NULL;break;}
			else {
				pmp=pm->prec;pms=pm->suiv;
				if (!pmp) teteM=pms;	//supprimer la tete
				if (pmp) pmp->suiv=pms; //prec non vide
				if (pms) pms->prec=pmp;	//suiv non vide
				free(pm);
				break;
			}
		}
		pm=pm->suiv;
	}
	pm=teteM;
	while (pm) { //Rremettre enVie=NON de chaque machine
		pm->enVie=NON;
		pm=pm->suiv;
	}
	affichage(teteM);
	printf("Fin de detecter\n\nAttendre...\n");fflush(stdout);
}

void init(){
	ID_MACHINE=getIdMachine();
	/*********************************** init signal ************************************/
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGUSR1);
	sigdelset(&ens,SIGINT);
	sigprocmask(SIG_SETMASK,&ens,NULL);
		
	struct sigaction act;
	act.sa_handler=sighand;
	act.sa_flags=SA_RESTART;
	sigaction(SIGUSR1,&act,0);
	/* set timer */
	set_ticker(30);
	/*********************************** init socket ************************************/
	/* Creating UDP socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Erreur ecouteur init : socket\n");exit(1);}
	/* remplir le nom local */
	memset((char *)&loc,0,sizeof(loc));
	loc.sin_family=AF_INET;
	loc.sin_port=htons(PortE);
	loc.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&loc, sizeof(struct sockaddr)) == -1) {
		perror("Erreur ecouteur init : bind\n");
		exit(1);
	}
	printf("Fin init\n");
}

void traitement(msg msgR) {
	sigLock();
	char add[15],ss[MAXDATA];
	int id=-1;
	printf("depuis addr %s\n",inet_ntoa(recu.sin_addr));
	strcpy(add,inet_ntoa(recu.sin_addr));
	switch (msgR.type) {
		case 1 : /* cas rexecute */
				printf("C'est 1 demande rexecute\n");
				if ((id=fork())>0) { //proc pere
					//ajouter ce proc dans les listes	
					ajouterProcE(add,getUID(ID_MACHINE,id),NON,-1);}
				else { //proc fils
					char code[MAXBUFF];
					strcpy(code,msgR.data);
					system(code);
					exit(0);}
				//envoyer UID
				memset(ss,0,sizeof(ss));
				sprintf(ss,"%d",getUID(ID_MACHINE,id));
				envoyer(11,ss,sock,recu);
				printf("Envoyer UID %d\n",atoi(ss));
				break;
			case 2 : /* cas rkill */
				printf("C'est 1 demande rkill\n");
				if ((id=fork())>0) { //proc pere
					int statRkill;
					waitpid(id,&statRkill,0);
					//envoyer la reponse vers demandeur
					memset(ss,0,sizeof(ss));
					if (statRkill) //cas erreur
						ss[0]='1';
					else ss[0]='0';
					envoyer(22,ss,sock,recu);}
				else { //proc fils
					//msgR.data contien "UID;numSIG"
					char buff[13],theU[10],theS[3];
					strcpy(buff,msgR.data);
					int t=0,i;
					while (buff[t]!=';') {
						theU[t]=buff[t];
						t++;}
					t++;
					for (i=t;i<strlen(buff);i++) {theS[i-t]=buff[i];}
					int nSig=atoi(theS),nUid=atoi(theU);
					//chercher le UID du processus, puis l'envoyer SIG
					if (chercherParUID(nUid,teteM)) {
						kill(getPID(ID_MACHINE,nUid),nSig);
						printf("kill(%d,%d)\n",getPID(ID_MACHINE,nUid),nSig);}
					else exit(1);
					exit(0);}
				break;
			case 3 : /* cas rwait */
				printf("C'est 1 demande rwait\n");
				int statRwait,pid=-1;
				if (chercherParADDR(add,teteM)) { //il y a de proc pour cette machine
					pProc ppt=chercherParFINI(add,teteM);
					if (!ppt) {
						printf("Non proc fini pour cette machine, wait...\n");
						while (1) { 
							pid=wait(&statRwait);
							printf("1 proc termine, ");
							ppt=chercherParUID(getUID(ID_MACHINE,pid),teteM);
							ppt->fini=OUI;
							ppt->etat=statRwait;
							if (strcmp(ppt->m->addr,add)==0) {printf("on le trouve\n");break;}
							else {printf("on continue chercher\n");}}}
					// Envoyer UID et Status
					char sEtat[5];
					memset(ss,0,sizeof(ss));
					sprintf(ss,"%d",ppt->uid);
					sprintf(sEtat,"%d",ppt->etat);
					strcat(ss,";");
					strcat(ss,sEtat);
					printf("data => %s\n",ss);
					envoyer(33,ss,sock,recu);
					printf("1 proc finit, je l'envoie\n");
					supprimer(ppt);
					printf("supprimer proc %d\n",ppt->uid);}
					else printf("Ancun proc pour cette machine\n");		
					/*
					if ((recvfrom(sock,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len))==-1) {
						printf("Erreur : recvfrom() \n");
						exit(1);
					}
					while (msgR.type!=333) {
						traitement();
						printf("Attendre...\n");
					}*/
				break;
			case 8 : //recu 1 msg pour compter nbr ecouteur
				printf("C'est 1 message pour compter nbr ecouteur\n");
				envoyer(88,"",sock,recu);
				break;
			case 9 : //recu 1 msg d'1 demandeur ->il est en vie
				printf("C'est 1 message pour detecter\n");
				pMach pm=teteM;
				while(pm) {
					if (strcmp(add,pm->addr)==0) {printf("trouver\n");pm->enVie=OUI;break;}
					pm=pm->suiv;}
				break;
			default : 
				printf("\n\nC'est 1 message type %d\n",msgR.type);
				break;
	}
	fflush(stdout);
	sigUnlock();
	affichage(teteM);
}
