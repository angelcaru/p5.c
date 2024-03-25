#include <p5.h>

typedef struct {
    int x, y;
    int dx, dy;
} Dvd;

#define SPEED 5

#define DVD_WIDTH 200
#define DVD_HEIGHT (DVD_WIDTH / 2)
#define WIDTH 640
#define HEIGHT 360

p5_Image img = {0};

void setup() {
    createCanvas(WIDTH, HEIGHT);
    loadImage("/web/assets/dvd.png", &img);
}

void draw() {
    static Dvd dvd = {
        .x = WIDTH / 2,
        .y = HEIGHT / 2,
        .dx = SPEED,
        .dy = SPEED,
    };

    if (!img.loaded) return;

    int newX = dvd.x + dvd.dx;
    int newY = dvd.y + dvd.dy;
    if (newX + DVD_WIDTH  >= WIDTH  || newX <= 0) dvd.dx *= -1;
    if (newY + DVD_HEIGHT >= HEIGHT || newY <= 0) dvd.dy *= -1;

    dvd.x = newX;
    dvd.y = newY;

    background(P5_WHITE);
    image(&img, dvd.x, dvd.y, DVD_WIDTH, DVD_HEIGHT);
}
