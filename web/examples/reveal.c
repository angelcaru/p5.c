#include <p5.h>

#define WIDTH 400
#define HEIGHT 400

p5_Image logo = {0};

p5_Graphics imgGr = {0};
p5_Graphics gr = {0};

#define REVEAL_SIZE 100
void setup(void) {
    loadImage("/web/assets/logo.png", &logo);

    createCanvas(WIDTH, HEIGHT);
    imgGr = createGraphics(WIDTH, HEIGHT);
    gr = createGraphics(REVEAL_SIZE, REVEAL_SIZE);
}

void draw(void) {
    background(P5_BLACK);

    if (!logo.loaded) return;
    graphicsBegin(&imgGr);
        background(P5_BLACK);
        image(&logo, 0, 0, WIDTH, HEIGHT);
    graphicsEnd();

    int mx = mouseX() - REVEAL_SIZE / 2, my = mouseY() - REVEAL_SIZE / 2, w = REVEAL_SIZE;
    graphicsBegin(&gr);
        copy(&imgGr, mx, my, w, w, 0, 0, w, w);

//        uint32_t pixels[REVEAL_SIZE*REVEAL_SIZE] = {0};
//        loadPixels(pixels);
//            for (int x = 0; x < w; x++) {
//                for (int y = 0; y < w; y++) {
//                    int dx = x - w/2;
//                    int dy = y - w/2;
//                    if (dx*dx + dy*dy > (w/2)*(w/2)) pixels[y*w + x] = 0;
//                }
//            }
//        updatePixels(pixels);
    graphicsEnd();

    copy(&gr, 0, 0, w, w, mx, my, w, w);
}
