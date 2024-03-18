#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include "networking.h"
#include "pong.h"

game G;

int main() {
    int sock;
    sock = setupHost();
    int flags = fcntl(sock, F_GETFL);
    //printf("%d\n", flags);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    char msg[] = "abcde";

    send(sock, msg, strlen(msg), 0);
    //
    //return 0;


    initWin(&G);

    G.leftPadStart = (HEIGHT - PADDLE_SIZE) / 2;
    G.rightPadStart = (HEIGHT - PADDLE_SIZE) / 2;

    G.bulletX = WIDTH/2;
    G.bulletY = HEIGHT/2;
    G.bulletVx = -1;
    G.bulletVy = 0;

    drawScreen(&G);

    uint8_t clock = 0;

    int buf[1];
    fd_set fdset;
    struct timeval timeout;
    size_t msgSize; 


    while (true) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        clock += 1;
        processKeyPress(&G, wgetch(G.win), 0);
       
        msgSize = sizeof buf;

        if (recv(sock, buf, msgSize, 0) > 0) {
            wmove(G.win, 10, 40);
            wprintw(G.win, "pressed: %d", buf[0]);
            processKeyPress(&G, buf[0], 1);
  //          wrefresh(G.win);
 //     //      usleep(5000000);
        //    break;
        }
        
        detectCollision(&G);

        G.bulletX += G.bulletVx;
        G.bulletY += G.bulletVy;

        

        send(sock, &G, sizeof (game), 0);

        drawScreen(&G);
        //wmove(G.win, 5, 35);
        //wprintw(G.win, "                 ");
        //wmove(G.win, 5, 35);
        //wprintw(G.win, "X: %d, Y: %d", G.bulletX, G.bulletY);
        //wrefresh(G.win);


        //wrefresh(G.win);
        flushinp();
        usleep(50000);
    }
    delwin(G.win);

    endwin();

    return 0;
}
