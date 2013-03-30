#include <stdio.h>   
#include <math.h>   
#include <string.h>   
#include <stdlib.h>   

typedef struct NOEUD {   
    int poids;   
    int Char;   
    int numNo;   
    struct NOEUD *pPere;   
    struct NOEUD *pFils[2]; /* pointeur vers 2 fils noeuds : 
									pFils[0] => fils gauche
									pFils[1] => fils droit */  
    struct BLOC *pbloc;   
    struct NOEUD *pPrev;   
}NOEUD,*PNOEUD;   
   
typedef struct BLOC {   
    PNOEUD pPrem;   /* le noeud qui a le plus grand poids dans ce bloc */
    struct BLOC *pPrev;   
    struct BLOC *pNext;   
}BLOC,*PBLOC;   
/* variables globales */
FILE *srcFile, *destFile,*sortie;  /* 	srcFile => fichier entree
											destFile => fichier sortie 
											sortie => pour l'affichage */
int  srcFileNO, destFileNO,startChar, charCount, E=0, R,tmp[256];   
char nomS[30];	/* pour le nom de fichier sortie */
unsigned char pEntree[128],pSortie[128];   
PNOEUD pCharNoeud[256],pNYT;   /*	pCharNoeud => ACSII 256 codes
										pNYT => # */
NOEUD racine;   
/* fonctions utilisees */
int afficher(PNOEUD pNoeud);   
int compression();   
void renouvellementBloc(PNOEUD pNoeud);   
void incrementer(PNOEUD pNoeud);   
void renouvellementNoeud(char c);   
/* *************************************************************** */
int main(int argc, char * argv[]) {   
	int i=1;   
	int isEncode = 1;   
	PBLOC pbloc, pPrevLINK;   
	PNOEUD  pNoeud,pNext;  
	srcFile = fopen(argv[1],"r"); 
	if (srcFile==NULL) {   
		printf("Il faut entrer le nom de fichier source ! \n");   
		exit(1);   
	}
    srcFileNO = fileno(srcFile);   
    rewind(srcFile);   /* mettre le pointeur au debut du fichier */
    
    strcpy(nomS,"CODE_SORTIE_COMP_BIS_");
    strcat(nomS,argv[1]);
    if ((destFile = fopen(nomS,"w"))==NULL) {   
        printf("Erreur : fopen destFile ! \n");   
        exit(1);   
    }   
    destFileNO = fileno(destFile);   
	if((sortie = fopen(argv[1],"r"))==NULL) {   
        printf("Erreur : fopen sortie ! \n");   
        exit(1);   
    }   
    /* init arbre huffman - racine */
	while(pow(2,(E+1))<=256) E++;   
	R = 256 - (int)pow(2,E);   
	racine.numNo = 2*256-1;   
	racine.pPere = NULL;   
	pNYT = &racine;   
   
	compression();   
	
    if(destFile) fclose(destFile);   
    if(srcFile) fclose(srcFile);   
    if(sortie) fclose(sortie);
    if(pNYT && pNYT != &racine) free(pNYT);   
    pbloc = racine.pbloc;   
    while(pbloc) {   
        pNoeud = pbloc->pPrem;   
        while(pNoeud) {   
            pNext = pNoeud->pPrev;   
            if(pNoeud != &racine)   
            free(pNoeud);   
            pNoeud = pNext;   
        }   
        pPrevLINK = pbloc->pPrev;   
        free(pbloc);   
        pbloc = pPrevLINK;   
    }   
	printf("\n\n%s est compresse dans le fichier '%s' par methode huffman diynamique\n",argv[1],nomS);    
	return EXIT_SUCCESS;
}   
/* *************************************************************** */  
int afficher(PNOEUD pNoeud) {   
    int len=0,marker=0;   
    char c;   
   
    fscanf(sortie,"%c",&c);   
    printf("\nla code Huffman de %c est :",c);
   
    if(pNoeud == pNYT)   
            marker=1;   
    while(pNoeud->pPere) {   
        tmp[len++] = (pNoeud->pPere->pFils[1] == pNoeud)?1:0;   
        pNoeud = pNoeud->pPere;   
    }   
    if(marker == 1)   
        tmp[0]=1;   
    while(len--) {   
		printf("%d",tmp[len]);   
        fprintf(destFile,"%d",tmp[len]);   
    }   
    return 1;   
}   
/* *************************************************************** */   
int compression() {   
    int bufLen,i;   
    while(1) {   
		bufLen = read(srcFileNO, pEntree, 128);
        if(!bufLen) break; /* en cas : lecture a la fin du fichier 
										  fin de traitement */
   
        for(i=0; i<bufLen; i++) {   
			if(!pCharNoeud[pEntree[i]]) {   
				if(pNYT != &racine)   
					if(!afficher(pNYT)) return 0;   
			}   
			else   
				if(!afficher(pCharNoeud[pEntree[i]])) return 0;   
			renouvellementNoeud(pEntree[i]); 
		}   
		return 1;   
	}   
}   
/* *************************************************************** */      
void renouvellementBloc(PNOEUD pNoeud) {   
    PBLOC pbloc = racine.pbloc;   
    PNOEUD  pNext;   
   
    if(!pbloc && racine.pFils[1]) {   
		pbloc = racine.pFils[1]->pbloc;   
        while(pbloc && pbloc->pNext)   
            pbloc = pbloc->pNext;   
    }   
	/* chercher le bloc pour ajouter pNoeud */
    while(pbloc && pbloc->pPrev && pbloc->pPrem->poids > pNoeud->poids) {   
        pbloc = pbloc->pPrev;   
    }   
	/* une fois sortie while : soit pbloc vide,soit le poids de bloc <= le poids de pNoeud
			CAS 1 => on trouve le bloc correspondant,
			si le poids de pblc = le poids de pNoeud */
    if(pbloc && pbloc->pPrem->poids == pNoeud->poids) {
		/* ajout ce noeud dans ce bloc */
        pNoeud->pbloc = pbloc;   
        /* si le num de ce noued > le num max de ce bloc => mettre ce noeud a pPrem */
        if(pNoeud->pbloc->pPrem->numNo < pNoeud->numNo) {   
            pNoeud->pPrev = pNoeud->pbloc->pPrem;   
            pNoeud->pbloc->pPrem = pNoeud;   
        }   
        else {
			/* chercher la bonne position a ajouter ce noeud */
            pNext = pNoeud->pbloc->pPrem;   
            while(pNext->pPrev && pNext->pPrev->numNo > pNoeud->numNo)   
                pNext = pNext->pPrev;   
            pNoeud->pPrev = pNext->pPrev;   
            pNext->pPrev = pNoeud;   
        }   
    }
    /* CAS 2 : soit bloc vide , le bloc dont poids correspondant absent
				 soit le poids de bloc < le poids de pNoeud */
    else { 
		/* allouer la memoire pour bloc correspondant */
        pNoeud->pbloc = (PBLOC) malloc(sizeof(BLOC));   
        pNoeud->pbloc->pPrem = pNoeud;   
        pNoeud->pPrev = NULL;   
        /* init le nouveau bloc */
        if(!pbloc) { /* bloc vide => 1er appel */ 
            pNoeud->pbloc->pPrev = NULL;   
            pNoeud->pbloc->pNext = NULL;   
        }   
		else /* soit le poids de bloc < le poids de pNoeud */
			if(pbloc->pPrem->poids < pNoeud->poids) {   
				pNoeud->pbloc->pPrev = pbloc;   
				pNoeud->pbloc->pNext = pbloc->pNext;   
			}   
			else { /* bloc absent, on cherche jusqu'a la fin(dernier bloc avec min poids), 
						mais le poids de pNoeud < POID min 
						=> mettre pNoeud a dernier */
				pNoeud->pbloc->pPrev = NULL;   
				pNoeud->pbloc->pNext = pbloc;   
			}   
		if(pNoeud->pbloc->pPrev) pNoeud->pbloc->pPrev->pNext = pNoeud->pbloc;   
		if(pNoeud->pbloc->pNext) pNoeud->pbloc->pNext->pPrev = pNoeud->pbloc;   
    }   
}   
/* *************************************************************** */ 
void incrementer(PNOEUD pNoeud)   
{   
    NOEUD noeud;   
    PNOEUD pNext; 
	/* si pNoeud est vide => 1er fois ajouter de noeud*/
    if(!pNoeud) return;   
    /* le noued a plus grand numNo dans le blos de pNoeud (P_max) n'est pas le pere de pNoeud 
		=> cas general */
    if(pNoeud->pbloc->pPrem != pNoeud->pPere) {  
		/* c'est le cas : on a besoin de echanger les noeuds */
        if(pNoeud->pbloc->pPrem->numNo > pNoeud->numNo) {
			/* la partie echange dans l'arbre */
			/* sauvegarder P_max */   
            noeud = *pNoeud->pbloc->pPrem;  
            /* P_max est le frere de pNoeud */ 
            if(pNoeud->pbloc->pPrem->pPere == pNoeud->pPere) {
				/* echanger */
                pNoeud->pPere->pFils[0] = (pNoeud->pPere->pFils[0] == pNoeud)?pNoeud->pbloc->pPrem:pNoeud;   
                pNoeud->pPere->pFils[1] = (pNoeud->pPere->pFils[1] == pNoeud)?pNoeud->pbloc->pPrem:pNoeud;   
            }   
            else {   
				/* pere de P_max n'est pas vide => P_max n'est pas racine */
                if(pNoeud->pbloc->pPrem->pPere) {  
					/* echanger */ 
					/* si P_max est le fils gauche */
                    if(pNoeud->pbloc->pPrem->pPere->pFils[0] == pNoeud->pbloc->pPrem)   
                        pNoeud->pbloc->pPrem->pPere->pFils[0] = pNoeud;   
                    else /* P_max est le fils droit */
                        pNoeud->pbloc->pPrem->pPere->pFils[1] = pNoeud;   
                }   
                /* pere de pNoeud n'est pas vide => pNoeud n'est pas racine */
                if(pNoeud->pPere) { 
					/*  pNoeud est fils gauche */
                    if(pNoeud->pPere->pFils[0] == pNoeud)   
                        pNoeud->pPere->pFils[0] = pNoeud->pbloc->pPrem;   
                    else   /*  pNoeud est fils droit */
                        pNoeud->pPere->pFils[1] = pNoeud->pbloc->pPrem;   
                }   
                pNoeud->pbloc->pPrem->pPere = pNoeud->pPere;   
                pNoeud->pPere = noeud.pPere;   
            }   
            /* la partie echange dans le bloc */
            /* pNext = P_max */
            pNext = pNoeud->pbloc->pPrem;   
            /* chercher pNoeud dans le bloc */
            while(pNext->pPrev->numNo > pNoeud->numNo) pNext = pNext->pPrev;   
            /* numNo de pNoeud est bien max, echanger pas */
            if(pNext == pNoeud->pbloc->pPrem) {   
				/* mettre pNoeud au Prem de son bloc */
                pNoeud->pbloc->pPrem->pPrev = pNoeud->pPrev;   
                pNoeud->pPrev = pNoeud->pbloc->pPrem;   
            }   
            else { /* on a besoin de echanger P_max <=> pNoeud */
                pNoeud->pbloc->pPrem->pPrev = pNoeud->pPrev;   
                pNext->pPrev = pNoeud->pbloc->pPrem;   
                pNoeud->pPrev = noeud.pPrev;   
            }   
            pNoeud->pbloc->pPrem->numNo = pNoeud->numNo;   
            pNoeud->numNo = noeud.numNo;   
   
            pNoeud->pbloc->pPrem = pNoeud;   
        }         
        /* apres echanger, pNoued a plus grand numNo
          mettre le suivant noeud au pPrem de bloc (pNoeud vas etre ajouter dans l'autre bloc)*/
        if(pNoeud->pPrev) pNoeud->pbloc->pPrem = pNoeud->pPrev;   
        else {  /* ce bloc contient un seul noeud => pNoeud 
					=> supprimer ce bloc */
            if(pNoeud->pbloc->pPrev)   
                pNoeud->pbloc->pPrev->pNext = pNoeud->pbloc->pNext;   
   
            if(pNoeud->pbloc->pNext)   
                pNoeud->pbloc->pNext->pPrev = pNoeud->pbloc->pPrev;   
            free(pNoeud->pbloc);   
        }   
    }   
    else {   /* P_max est le pere de pNoeud => cas particulier 
				 supprimer pNoeud depuis le bloc */
        pNext = pNoeud->pbloc->pPrem;   
        /* chercher pNoeud */
        while(pNext->pPrev != pNoeud) pNext = pNext->pPrev;   
        pNext->pPrev = pNoeud->pPrev;   
    }   
    /* supprimer le champs bloc de pNoeud */
    pNoeud->pbloc = NULL;   
    pNoeud->poids++;   
    /* mise a jour le champs bloc de pNoeud */
    renouvellementBloc(pNoeud);   
    incrementer(pNoeud->pPere);   
}   
/* *************************************************************** */  
void renouvellementNoeud(char c) {   
    if(!pCharNoeud[c]) { /* element c n'est pas dans l'arbre => 1er fois entree */
		/* remplace pNYT par sous-arbre => fils gauche : pNYT
		 										fils droit : char c */
        pNYT->poids = 1;   
        pNYT->Char = -1;   
        pNYT->pFils[1] = (PNOEUD) malloc(sizeof(NOEUD));//nouvelle feuille  
        pNYT->pFils[0] = (PNOEUD) malloc(sizeof(NOEUD));//nouveau NYT   
		/* mettre les donnees de fils gauche */
        pNYT->pFils[0]->pPere = pNYT;   
        pNYT->pFils[0]->numNo = pNYT->numNo-2;   
        pNYT->pFils[0]->poids = 0;   
        pNYT->pFils[0]->Char = -1;   
        memset(pNYT->pFils[0]->pFils,0,sizeof(pNYT->pFils[0]->pFils));   
        pNYT->pFils[0]->pPrev = NULL;   
        pNYT->pFils[0]->pbloc = NULL;   
		/* mettre les donnees de fils droit */
        pNYT->pFils[1]->pPere = pNYT;   
        pNYT->pFils[1]->numNo = pNYT->numNo-1;   
        pNYT->pFils[1]->poids = 1;   
        pNYT->pFils[1]->Char = c;   
        memset(pNYT->pFils[1]->pFils,0,sizeof(pNYT->pFils[1]->pFils));   
        pNYT->pFils[1]->pPrev = NULL;   
        pNYT->pFils[1]->pbloc = NULL;   
		/* ajout c dans l'arbre */
        pCharNoeud[c] = pNYT->pFils[1];   
        pNYT = pNYT->pFils[0];   
   
        renouvellementBloc(pNYT->pPere);   
        pCharNoeud[c]->pbloc = pCharNoeud[c]->pPere->pbloc;   
        pCharNoeud[c]->pPere->pPrev = pCharNoeud[c];   
   
        incrementer(pCharNoeud[c]->pPere->pPere); /* le noeud a incrementer : son grand-pere*/
    }   
    else { /* element c est deja dans l'arbre */
        incrementer(pCharNoeud[c]); /* le noeud a incrementer : soi-meme */
    }   
}   
