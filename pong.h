#include <ncurses.h>

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

int processKeyPress(game *G, int c, int side);

void detectCollision(game *G);

void drawScreen(game *G);

void initWin(game *G);
