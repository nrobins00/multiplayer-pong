#include <ncurses.h>
#include <unistd.h>

#define HEIGHT 24
#define WIDTH 80
#define PADDLE_SIZE 4

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):((int)(x)-0.5))

typedef struct game {
    WINDOW *win;
    int leftPadStart;
    int rightPadStart;
    int leftPadV, rightPadV;
    int bulletX;
    float bulletY;
    int bulletVx;
    float bulletVy;
} game;

game G;

int floatToInt(float f) {
    if (f >= 0) return (int)(f+0.5);
    return (int)(f-0.5);
}

void drawPaddle(int startY, int rightSide) {
    int i;
    int col = rightSide == 1 ? WIDTH - 1 : 0;
    for (i = startY; i < startY + PADDLE_SIZE; i++) {
        mvwaddch(G.win, i, col, ' ' | A_REVERSE);
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
    wborder(win, ' ', ' ', ' ' | A_REVERSE, ' ' | A_REVERSE, ' ', ' ', ' ', ' ');

    G.win = win;
    wrefresh(win);
}

void processKeyPress(int c) {
    if (c == KEY_DOWN) {
        clearPaddle(0);
        G.leftPadStart++;
        G.leftPadV = 1;
        drawPaddle(G.leftPadStart, 0);
    } else if (c == KEY_UP) {
        clearPaddle(0);
        G.leftPadStart--;
        G.leftPadV = -1;
        drawPaddle(G.leftPadStart, 0);
    } else {
        //G.leftPadV = 0;
    }
}

int detectCollision() {
    int bulletX = G.bulletX;
    int bulletY = floatToInt(G.bulletY);
    if (bulletX == 1) {

        if (G.leftPadStart > bulletY || G.leftPadStart < bulletY - 4) {
        //    return 1;
        }
        if (bulletY == G.leftPadStart || bulletY == G.leftPadStart + 1) {
            G.bulletVy = -1;
        } else if (bulletY == G.leftPadStart + 2 || bulletY == G.leftPadStart + 3) {
            G.bulletVy = 1;
        }
        G.bulletVx = 1;
    }
    if (bulletX == WIDTH - 2) {
        if (G.rightPadStart > bulletY || G.rightPadStart < bulletY - 4) {
         //   return 1;
        }
        G.bulletVx = -1;
    }
    if (bulletY <= 1 || bulletY >= HEIGHT - 2) {
        G.bulletVy *= -1;
        wmove(G.win, 7, 35);
        wprintw(G.win, "hit at %d", bulletY);
    }

        return 0;
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
    G.bulletY = HEIGHT/2.0;
    G.bulletVx = -1;
    mvwaddch(G.win, G.bulletY, G.bulletX, '~' | ACS_BULLET);

    uint8_t clock = 0;

    while (true) {
        clock += 1;
        processKeyPress(wgetch(G.win));
        detectCollision();

        mvwaddch(G.win, floatToInt(G.bulletY), G.bulletX, ' ');
        G.bulletX += G.bulletVx;
        G.bulletY += G.bulletVy;
        mvwaddch(G.win, floatToInt(G.bulletY), G.bulletX, ' ' | A_REVERSE);
        wmove(G.win, 5, 35);
        wprintw(G.win, "                 ");
        wmove(G.win, 5, 35);
        wprintw(G.win, "X: %d, Y: %d", G.leftPadStart, floatToInt(G.bulletY));
        
        wrefresh(G.win);
        flushinp();
        usleep(50000);
    }
    delwin(G.win);

    endwin();

    return 0;
}
    
