#define HEIGHT 24
#define WIDTH 80
#define PADDLE_SIZE 4

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
