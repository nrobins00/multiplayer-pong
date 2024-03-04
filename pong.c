#include <ncurses.h>

#define HEIGHT 30
#define WIDTH 90
#define PADDLE_SIZE 4

typedef struct game {
    WINDOW *win;
} game;

game G;

void drawPaddle(int startY, int rightSide) {
    int i;
    int col = rightSide == 1 ? WIDTH - 1 : 0;
    for (i = startY; i < startY + PADDLE_SIZE; i++) {
        mvwaddch(G.win, i, col, 'x' | ACS_VLINE);
    }
    wrefresh(G.win);
}

void initWin() {
    WINDOW *win = newwin(HEIGHT, WIDTH, 0, 0);
    wborder(win, ' ', ' ', 0, 0, ' ', ' ', ' ', ' ');

    G.win = win;
    wrefresh(win);
}

int main() {
    initscr();
    cbreak();
    noecho();
    refresh();

    initWin();
    drawPaddle((HEIGHT - PADDLE_SIZE) / 2, 0);
    drawPaddle((HEIGHT - PADDLE_SIZE) / 2, 1);

    getch();

    delwin(G.win);
    endwin();

    return 0;
}
    
