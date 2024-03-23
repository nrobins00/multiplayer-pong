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
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    char msg[] = "abcde";

    send(sock, msg, strlen(msg), 0);

    initWin(&G);

    G.leftPadStart = (HEIGHT - PADDLE_SIZE) / 2;
    G.rightPadStart = (HEIGHT - PADDLE_SIZE) / 2;

    G.bulletX = WIDTH/2;
    G.bulletY = HEIGHT/2;
    G.bulletVx = -1;
    G.bulletVy = 0;
    G.leftScore = 0;
    G.rightScore = 0;

    drawScreen(&G);

    uint8_t clock = 0;

    int buf[1];
    fd_set fdset;
    struct timeval timeout;
    size_t msgSize; 


    while (!G.finished) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        clock += 1;
        if (processKeyPress(&G, wgetch(G.win), 0)) {
            //host user quit
        }
       
        msgSize = sizeof buf;

        if (recv(sock, buf, msgSize, 0) > 0) {
            if (processKeyPress(&G, buf[0], 1)) {
                //remote user quit
            }
        }
        

        if (clock % 5 == 0) {
            G.bulletX += G.bulletVx;
            G.bulletY += G.bulletVy;
            detectCollision(&G);
        }

        send(sock, &G, sizeof (game), 0);

        drawScreen(&G);

        flushinp(); //clears input buffer
        usleep(10000);
    }
    delwin(G.win);

    endwin();

    return 0;
}
