/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <ncurses.h>

#include "networking.h"
#include "pong.h"

#define SERVERPORT "4951"	// the port users will be connecting to
#define MAXBUFLEN 100

game G;

// get sockaddr, IPv4 or IPv6:

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char buf[MAXBUFLEN];
    struct sockaddr_storage my_addr;
    char s[INET6_ADDRSTRLEN];

	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        perror("talker: socket");
        exit(1);
    }

	freeaddrinfo(servinfo);


	if ((numbytes = send(sockfd, argv[2], strlen(argv[2]), 0))== -1) {
		perror("talker: sendto");
		exit(1);
	}


	printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);

//    usleep(1000000);

    addr_len = sizeof their_addr;
    if ((numbytes = recv(sockfd, buf, MAXBUFLEN - 1, 0)) == -1) {
		perror("recvfrom");
		exit(1);
    }

    int flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	//buf[numbytes] = '\0';
	//printf("talker: packet contains \"%s\"\n", buf);

    initWin(&G);
    int c;

    game hostG;
    game gameBuf[1];
    ssize_t recvAmt;
    int cyclesWithNo;
    
    while (!G.finished) {
       c = wgetch(G.win); 

       if (c == KEY_UP || c == KEY_DOWN || c == 'q') {
            if ((numbytes = send(sockfd, &c, sizeof (int), 0)) == -1) {
                perror("talker: sendto");
                exit(1);
            }
            wprintw(G.win, "sent: %d", c);
        }

       memset(&gameBuf, 0, sizeof gameBuf);

       recvAmt = recv(sockfd, &gameBuf, sizeof (gameBuf), 0);
       //TODO: SEND END SIGNAL FROM HOSt or quit if haven't received signal after certain num of cycles
        int numRecvd = recvAmt / sizeof(hostG);
        if (recvAmt > 0) {
            hostG = gameBuf[0];
           G.bulletX = hostG.bulletX;
           G.bulletY = hostG.bulletY;
           G.leftPadStart = hostG.leftPadStart;
           G.rightPadStart = hostG.rightPadStart;
           G.leftScore = hostG.leftScore;
           G.rightScore = hostG.rightScore;
           G.finished = hostG.finished;
        }

       drawScreen(&G);
        wrefresh(G.win);
       usleep(10000);

    }

    delwin(G.win);
    endwin();

	close(sockfd);

    printf("recvAmt: %lu, sizeof gameBuf: %lu, \n", recvAmt, sizeof gameBuf);

	return 0;
}
