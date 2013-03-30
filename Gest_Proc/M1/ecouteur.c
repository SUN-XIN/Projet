#include "../include/ecouteur.h"

int main(){
	init();
	msg msgR;
	
	fd_set fd;
	struct timeval timeout;
	timeout.tv_sec=3;
	timeout.tv_usec=0;
	int s;
	printf("\nAttendre...\n");
	while(1) {
		FD_ZERO(&fd); 
		FD_SET(sock,&fd);
		s = select(sock+1, &fd, NULL, NULL, &timeout);
		if (s>0) {
			msgR.type=-1;strcpy(msgR.data,"");
			recvfrom(sock,&msgR,sizeof(msgR),0,(struct sockaddr *)&recu,&len);
			printf("Recu 1 msg ");
			traitement(msgR);
			printf("\nAttendre...\n");}
		else {}
	}
	return EXIT_SUCCESS;
}
