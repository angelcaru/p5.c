#include <p5.h>

#define WIDTH  500
#define HEIGHT 500

void setup(void) {
    createCanvas(WIDTH, HEIGHT);
}

void branch(int len, float angle, int depth) {
    line(0, 0, 0, -len);
    translate(0, -len);
    if (depth > 0) {
        push();
            rotate(angle);
            branch(len * 0.75, angle, depth - 1);
        pop();
        push();
            rotate(-angle);
            branch(len * 0.75, angle, depth - 1);
        pop();
    }
}

void draw(void) {
    static float angle = 0.01f;
    background(P5_WHITE);
    angle += 0.01f;
    translate(WIDTH/2, HEIGHT);
    branch(100, angle, 10);
}
