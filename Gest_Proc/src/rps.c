#include "strcut_donnee.h"

int main() {
	int sock; 
	struct sockaddr_in dest,recu,loc,mul;
	socklen_t len=sizeof(struct sockaddr_in);
	msg msgE,msgR;
	/* Creating UDP socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Erreur ecouteur init : socket\n");
		exit(1);
	}
	/* remplir le nom local */
	memset((char *)&loc,0,sizeof(loc));
	loc.sin_family=AF_INET;
	loc.sin_port=htons(PortR);
	loc.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&loc, sizeof(struct sockaddr)) == -1) {
		perror("Erreur rps init : bind\n");
		exit(1);
	}
	/* remplir le nom de mul */
	memset((char *)&mul,0,sizeof(mul));
	mul.sin_family=AF_INET;
	mul.sin_port=htons(PortD);
	mul.sin_addr.s_addr = inet_addr(mulAddrRPS);
	//envoyer 1 msg vers mulcast
	msgE.type=7;
	strcpy(msgE.data,"");
	sendto(sock,&msgE,sizeof(msgE),0,(struct sockaddr *)&mul,len);
	//attendre les reponses des demandeur pendant 5s
	fd_set fd;
	struct timeval timeout;
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	FD_ZERO(&fd); 
	FD_SET(sock,&fd);
	int s;
	char r[MAXBUFF];
	while(1) {
	   s = select(sock+1, &fd, NULL, NULL, &timeout);
	   if (s>0) {
			recvfrom(sock,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len);
			printf("Recu 1 reponse d'1 demandeur '%s'\n",inet_ntoa(recu.sin_addr));
			strcpy(r,msgR.data);
			int i=0;
			printf("    add %s     uid ", inet_ntoa(recu.sin_addr));
			while (r[i]!='F') {
				if (r[i]==';') printf("\n    add %s     uid ", inet_ntoa(recu.sin_addr));
				else printf("%c",r[i]);
				i++;
			}
			printf("\n");
	   }else break;
	   sleep(1); 
	}
	return EXIT_SUCCESS;
}
