#include <p5.h>

#define WIDTH  800
#define HEIGHT 600
void setup(void) {
    createCanvas(WIDTH, HEIGHT);
    pixelDensity(1);
}

void draw(void) {
    background(255, 0, 0);

    static uint32_t pixels[WIDTH * HEIGHT] = {0};
    loadPixels(pixels);
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                pixels[y * WIDTH + x] = color(0, x * 255 / WIDTH, y * 255 / HEIGHT, 255);
            }
        }
    updatePixels(pixels);
}
