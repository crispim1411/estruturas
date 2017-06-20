#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ncurses.h>


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char username[10];
    char buffer[256], auxbuffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    // Getting a username
    printf("Enter your username: ");
    fgets(username, 7, stdin);
    for (n = 0; n < strlen(username); n++) {
        if (username[n] == '\n') {
            username[n] = ':';
            username[n+1] = ' ';
            username[n+2] = '\0';
            break;
        }
    }


    // WINDOWS SETTINGS
    // -- Parameters --
    WINDOW *top;
    WINDOW *bottom;
    int line=1; // line position of top
    int input=1; // line position of top
    int maxx, maxy; // screen dimensions

    // -- Setup Windows --
    initscr();
    getmaxyx(stdscr, maxy, maxx); // get max values

    top = newwin(maxy-3, maxx, 0, 0); // create top window
    bottom = newwin(3, maxx, maxy-3, 0); // create bottom window

    scrollok(top, TRUE);
    scrollok(bottom, TRUE);
    box(top, '|', '=');
    box(bottom, '|', '-');

    wsetscrreg(top, 1, maxy-3);
    wsetscrreg(bottom, maxy-3, maxy);

    //refresh();
    wrefresh(top);
    wrefresh(bottom);
    //
    //

    while (1) {
        //printf("Please enter the message: ");
        mvwprintw(bottom, 1, 2, "                               ");
        mvwprintw(bottom, 1, 2, "%s", username);
        wrefresh(bottom);
        bzero(buffer,256);
        bzero(auxbuffer, 256);
        //fgets(buffer,255,stdin);
        wgetnstr(bottom, buffer, 255);
        strcat(auxbuffer, username);
        strncat(auxbuffer, buffer, 200);
        //
        //
        n = write(sockfd,auxbuffer,strlen(auxbuffer));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256);
        //puts("1");
        n = read(sockfd,buffer,255);
        //puts("2");
        if (n < 0) {
            endwin();
            error("ERROR reading from socket");
        }
        
        //printf("%s\n",buffer);
        mvwprintw(top, 1, 2, "%s\n", buffer);
        wrefresh(top);
    }
    endwin();

    return 0;
}
