#include <arpa/inet.h>
#include <aio.h>
#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/ioctl.h>

typedef struct processus{
	int uid;
	int fini; // OUI ou NON
	int etat; // status lors qu'il est fini ou -1 s'il est pas encore fini
	struct processus *prec;
	struct processus *suiv;
	struct machine *m; //cree par machine m
}proc,*pProc;

typedef struct machine{
	char addr[15];
	int nbrProc;
	int enVie;
	struct machine *prec;
	struct machine *suiv;
	struct processus *tp; //tete : @ de premier proc
}mach,*pMach;

typedef struct message{
	int type;	/* msg.type	1 - rexecute 	11 - reponse de rexecute
							2 - rkill		22 - reponse de rkill
							3 - rwait 		33 - reponse de rwait*/
	char data[30];
}msg;

#define mulAddr "224.0.0.1"
#define mulAddrRPS "224.0.0.2"

#define PortD 7100	//port demandeur
#define PortE 7200	//port ecouteur
#define PortR 7300	//port rps

#define NON 0
#define OUI 1
#define MAXBUFF 128
#define MAXDATA 30
#define t_uid int

socklen_t len=sizeof(struct sockaddr_in);

void sigLock() {
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGINT);
	sigprocmask(SIG_SETMASK,&ens,NULL);
	printf("\n*********\nLock\n");
}

void sigUnlock() {
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGUSR1);
	sigdelset(&ens,SIGINT);
	//sigdelset(&ens,SIGALRM);
	sigprocmask(SIG_SETMASK,&ens,NULL);
	printf("Unlock\n*********\n");
	sleep(1);
}

void set_ticker(int periode) {
	timer_t tt;
	struct sigevent timer_event;
	struct itimerspec timer;

	timer.it_interval.tv_sec=periode; 
    timer.it_interval.tv_nsec=0;
	timer.it_value = timer.it_interval;

	timer_event.sigev_notify = SIGEV_SIGNAL;
	timer_event.sigev_signo = SIGUSR1;
	timer_event.sigev_value.sival_ptr = (void *)&tt;

	if (timer_create (CLOCK_REALTIME, &timer_event, &tt) < 0) {
		printf ("Erreur : timer_create\n");
		exit (1);
	}

	if (timer_settime (tt, 0, &timer, NULL) < 0) {
		printf ("Erreur : timer_settime\n");
		exit (1);
	}
}  

int getIdMachine(){
	int fd;
    struct ifreq ifr;
	char theAdr[16];
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    ioctl(fd, SIOCGIFADDR, &ifr);
    strcpy(theAdr,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    close(fd);
    int id,i,k;
    char cId[4];
    strcpy(cId,"");
    for (i=strlen(theAdr)-1;theAdr[i]!='.';i--) {}
    k=i+1;
	for (i=k;i<strlen(theAdr);i++) cId[i-k]=theAdr[i];
    printf("Machine ID %d\n",atoi(cId));
	return atoi(cId);
}

int getUID(int numM,int numP) {
	char codeM[MAXBUFF],codeP[MAXBUFF];
	sprintf(codeM,"%d",numM);
	sprintf(codeP,"%d",numP);
	strcat(codeM,codeP);
	return atoi(codeM);
}

int getPID(int numM,int numU) {
	int tmp=10;
	while (tmp*numM*10<numU) {tmp=tmp*10;}
	return (numU-tmp*numM);
}

void affichage(pMach tete){
	printf("|    address machine     UID     FINI  |\n");
	pMach pm=tete;
	pProc pp=NULL;
	while (pm){
		if (!pm->nbrProc) {
			printf("|%19s     NULL          |\n",pm->addr);
			pm=pm->suiv;
			continue;
		}
		pp=pm->tp;
		while (pp){
			printf("|%19s%10d%5d    |\n",pm->addr,pp->uid,pp->fini);
			pp=pp->suiv;
		}
		pm=pm->suiv;
	}
}

pMach chercherParADDR(char *add,pMach tete){
	pMach pm=tete;
	while (pm) {
		if (strcmp(pm->addr,add)!=0) {pm=pm->suiv;continue;}
		if ((pm->nbrProc)<1) return NULL;
		return pm;
	}
}

pProc chercherParFINI(char *add,pMach tete){
	pMach pm=NULL;
	pProc pp=NULL;
	if (strlen(add)>4) {
		pm=chercherParADDR(add,tete);
		pp=pm->tp;
		do {
			if (pp->fini==OUI) return pp; 
			pp=pp->suiv;
		}while (pp);}
	else {
		pm=tete;
		while(pm) {
			pp=pm->tp;
			while(pp) {
				if (pp->fini==OUI) return pp;
				pp=pp->suiv;}
			pm=pm->suiv;}}
	return NULL;
}

void supprimer(pProc thePp){
	pProc pp,ppp,pps;
	pMach pm=thePp->m;
	if (pm->nbrProc==1) {
		pm->nbrProc=0;
		free(pm->tp);
	}
	else {
		pm->nbrProc--;
		pp=pm->tp;
		while (pp!=thePp) {pp=pp->suiv;}
		ppp=pp->prec;pps=pp->suiv;
		if (!ppp) { //supprimer la tete
			pm->tp=pps;
		}
		if (ppp) ppp->suiv=pps; //prec non vide
		if (pps) pps->prec=ppp; //suiv non vide
		free(pp);
	}
}

pProc chercherParUID(int theUID,pMach tete){
	pProc pp=NULL;
	pMach pm=tete;
	while(pm) {
		if ((pm->nbrProc)<1) {pm=pm->suiv;continue;}
		pp=pm->tp;
		do {
			if (pp->uid==theUID) return pp;
			pp=pp->suiv;
		}while (pp);
		pm=pm->suiv;
	}
	return NULL;
}

void envoyer(int type,char *data,int theSock,struct sockaddr_in dest){
	msg msgE;
	//memset(msgE,0,sizeof(msg));
	msgE.type=type;
	strcpy(msgE.data,data);
	sendto(theSock,&msgE,sizeof(msgE),0,(struct sockaddr *)&dest,len);
}

/*
 * void ajouterProcE(char *add,int theUID,int theFINI,int theETAT,pMach tete,int *nbrMachine) {
	pProc pp;
	pMach pm=tete;
	if (!tete) { //tete de liste machine est vide=>la premiere fois ajouter
		printf("Tete vide -> 1er machine ajoutée dans la liste\n");
		*nbrMachine=1;
		tete=(pMach)(malloc(sizeof(mach)));
		strcpy(tete->addr,add);
		tete->enVie=OUI;
		tete->prec=NULL;
		tete->suiv=NULL;
		if (theUID>0) {
			tete->nbrProc=1;
			pp=(pProc)(malloc(sizeof(proc)));
			pp->uid=theUID;
			pp->fini=theFINI;
			pp->etat=theETAT;
			pp->suiv=NULL;
			pp->prec=NULL;
			pp->m=tete;
			tete->tp=pp;
		}
		else {
			tete->enVie=OUI;
			tete->nbrProc=0;
			tete->tp=NULL;
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
			(*nbrMachine)++;
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
}*/



