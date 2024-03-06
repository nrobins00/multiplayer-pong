#include <ncurses.h>
#include <unistd.h>

#define HEIGHT 30
#define WIDTH 90
#define PADDLE_SIZE 4

typedef struct game {
    WINDOW *win;
    int leftPadStart;
    int rightPadStart;
    int bulletX, bulletY;
} game;

game G;

void drawPaddle(int startY, int rightSide) {
    int i;
    int col = rightSide == 1 ? WIDTH - 1 : 0;
    for (i = startY; i < startY + PADDLE_SIZE; i++) {
        mvwaddch(G.win, i, col, 'x' | ACS_VLINE);
    }
}

void clearPaddle(int side) {
    int col = side == 1 ? WIDTH - 1 : 0;
    int start = side == 1 ? G.rightPadStart : G.leftPadStart;
    int i;
    for (i = start; i < start + PADDLE_SIZE; i++) {
        mvwaddch(G.win, i, col, ' ');
    }
}

void initWin() {
    WINDOW *win = newwin(HEIGHT, WIDTH, 0, 0);
    wborder(win, ' ', ' ', 0, 0, ' ', ' ', ' ', ' ');

    G.win = win;
    wrefresh(win);
}

void processKeyPress(int c) {
    if (c == KEY_DOWN) {
        clearPaddle(0);
        G.leftPadStart++;
        drawPaddle(G.leftPadStart, 0);
    } else if (c == KEY_UP) {
        clearPaddle(0);
        G.leftPadStart--;
        drawPaddle(G.leftPadStart, 0);
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();

    initWin();
    wtimeout(G.win, 0);
    nodelay(G.win, TRUE);
    keypad(G.win, TRUE);
    G.leftPadStart = (HEIGHT - PADDLE_SIZE) / 2;
    G.rightPadStart = (HEIGHT - PADDLE_SIZE) / 2;
    drawPaddle(G.leftPadStart, 0);
    drawPaddle(G.rightPadStart, 1);

    G.bulletX = WIDTH/2;
    G.bulletY = HEIGHT/2;
    mvwaddch(G.win, G.bulletY, G.bulletX, '~' | ACS_BULLET);
    

    while (true) {
        processKeyPress(wgetch(G.win));
        G.bulletX--;
        mvwaddch(G.win, G.bulletY, G.bulletX, '~' | ACS_BULLET);
        wrefresh(G.win);
        usleep(100000);
    }
    delwin(G.win);

    endwin();

    return 0;
}
    
