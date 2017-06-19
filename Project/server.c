/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define ETH_INTERFACE "enp63s0"

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     struct ifreq ifr;

     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        error("ERROR opening socket");

     ifr.ifr_addr.sa_family = AF_INET;
     strncpy(ifr.ifr_name, ETH_INTERFACE, IFNAMSIZ-1);
     ioctl(sockfd,SIOCGIFADDR, &ifr);
     printf("IP: %s\n",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	  error("ERROR on binding");
     listen(sockfd,5);
     puts("1");
     clilen = sizeof(cli_addr);
     puts("2");
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     puts("3");
     while(1){
	     if (newsockfd < 0) 
		  error("ERROR on accept");
	     bzero(buffer,256);
	     n = read(newsockfd,buffer,255);
	     if (n < 0) error("ERROR reading from socket");
	     //printf("Here is the message: %s\n",buffer);
	     printf("%s\n",buffer);
	     n = write(newsockfd,"I got your message",18);
	     if (n < 0) error("ERROR writing to socket");
     }
     return 0; 
}
