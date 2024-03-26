#include <p5.h>

// draw a spinning box
// with width, height and depth of 50
void setup(void) {
    createCanvas3d(100, 100);
    camera1(0, 0, -1);
    //camera(0, 0, 50*sqrt(3.0f), 0, 0, 0, 0, 1, 0);
    //perspective(PI/3, 1, 5*sqrt(3.0f), 500*sqrt(3.0f));
}

void draw(void) {
    static int frameCount = 0;
    background(200, 200, 200);
    rotateX(frameCount * 0.01f);
    rotateY(frameCount * 0.01f);
    box(50, 50, 50);

    frameCount++;
}
