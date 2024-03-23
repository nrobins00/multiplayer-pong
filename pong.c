#include <ncurses.h>
#include <unistd.h>
#include <sys/select.h> 
#include <sys/time.h>
#include <fcntl.h>
#include <sys/socket.h>


#define HEIGHT 24
#define WIDTH 80
#define PADDLE_SIZE 4

typedef struct game {
    WINDOW *win;
    int leftPadStart;
    int rightPadStart;
    int leftPadV, rightPadV;
    int bulletX, bulletY, bulletVx, bulletVy;
    int leftScore, rightScore, finished;
} game;

void drawPaddle(WINDOW *win, int startY, int rightSide) {
    int i;
    int col = rightSide == 1 ? WIDTH - 1 : 0;
    for (i = startY; i < startY + PADDLE_SIZE; i++) {
        mvwaddch(win, i, col, ' ' | A_REVERSE);
    }
}

void initWin(game *G) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    WINDOW *win = newwin(HEIGHT, WIDTH, 0, 0);
    wborder(win, ' ', ' ', ' ' | A_REVERSE, ' ' | A_REVERSE, ' ', ' ', ' ', ' ');

    wtimeout(win, 0);
    nodelay(win, TRUE);
    keypad(win, TRUE);

    G->win = win;
}

void movePaddle(int *padStart, int down) {
    int newPadStart = *padStart;
    if (down) {
        newPadStart++;
    } else {
        newPadStart--;
    }
    if (newPadStart >=0 && newPadStart <= (HEIGHT - PADDLE_SIZE)) {
        *padStart = newPadStart;
    }
}

int processKeyPress(game *G,int c, int player) {
    int padStart;
    switch (c) {
        case KEY_DOWN:
        case KEY_UP:
            if (player == 0) {
                movePaddle(&G->leftPadStart, c == KEY_DOWN);
            } else {
                movePaddle(&G->rightPadStart, c == KEY_DOWN);
            }
            break;
        case 'q':
            G->finished = 1;
            return 1;
    }
    return 0;
}

void handleGoal(game *G, int leftScored) {
    if (leftScored) {
        G->leftScore++;
    } else {
        G->rightScore++;
    }
    G->bulletX = WIDTH / 2;
    G->bulletY = HEIGHT / 2;
    G->bulletVx = -1;
    G->bulletVy = 0;
}

int detectCollision(game *G) {
    int bulletX = G->bulletX;
    int bulletY = G->bulletY;
    if (bulletX == 1) {
        if (G->leftPadStart > bulletY || G->leftPadStart < bulletY - 4) {
            handleGoal(G, 0);
            return 1;
        }
        if (bulletY == G->leftPadStart || bulletY == G->leftPadStart + 1) {
            G->bulletVy = -1;
        } else if (bulletY == G->leftPadStart + 2 || bulletY == G->leftPadStart + 3) {
            G->bulletVy = 1;
        }
        G->bulletVx = 1;
    }
    if (bulletX == WIDTH - 2) {
        if (G->rightPadStart > bulletY || G->rightPadStart < bulletY - 4) {
            handleGoal(G, 1);
            return 1;
        }
        if (bulletY == G->rightPadStart || bulletY == G->rightPadStart + 1) {
            G->bulletVy = -1;
        } else if (bulletY == G->rightPadStart + 2 || bulletY == G->rightPadStart + 3) {
            G->bulletVy = 1;
        }
        G->bulletVx = -1;
    }
    if (bulletY <= 1 || bulletY >= HEIGHT - 2) {
        G->bulletVy *= -1;
    }

        return 0;
}

void drawScreen(game *G) {
    werase(G->win);
    wborder(G->win, ' ', ' ', ' ' | A_REVERSE, ' ' | A_REVERSE, ' ', ' ', ' ', ' ');
    drawPaddle(G->win, G->leftPadStart, 0); 
    drawPaddle(G->win, G->rightPadStart, 1); 
    mvwaddch(G->win, G->bulletY, G->bulletX, ' ' | A_REVERSE);
    wmove(G->win, 7, WIDTH / 4);
    wprintw(G->win, "%d", G->leftScore);
    wmove(G->win, 7, 3 * WIDTH / 4);
    wprintw(G->win, "%d", G->rightScore);
    wrefresh(G->win);
}

    
