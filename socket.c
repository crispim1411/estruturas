#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

int main(int argc, char *argv[])
{	
	int meusocket = socket(AF_INET,SOCK_STREAM,6);
	int conector;

	typedef struct {
		short int family;	
		unsigned short int porta;
		struct in_addr sin_addr;
		unsigned char zero[8];
	} sockaddr_in;
	
	sockaddr_in servidor;
	
	if(argc < 2){
		fprintf(stderr,"No port provided\n");
		exit(1);
	}

	int portnum = atoi(argv[1]);
	
	if(meusocket < 0){
		perror("Socket");
		exit(1);
	}

	servidor.family = AF_INET;
	servidor.porta = htons(portnum);
	servidor.sin_addr.s_addr = INADDR_ANY;
        bzero(&(servidor.zero),8);

	conector = connect(meusocket,(struct sockaddr * )&servidor, sizeof(servidor));
	if(conector < 0){
		perror("Connect");
		exit(1);
	}
	return 0;
}
