// Apapted from https://p5js.org/examples/structure-create-graphics.html

#include <p5.h>

p5_Graphics pg = {0};

#define WIDTH  710
#define HEIGHT 400
void setup(void) {
    createCanvas(WIDTH, HEIGHT);
    pg = createGraphics(400, 250);
}

#define CIRCLE_SIZE 60
void draw(void) {
    fillA(P5_BLACK, 12);
    rect(0, 0, WIDTH, HEIGHT);
    fill(P5_WHITE);
    noStroke();
    int mx = mouseX();
    int my = mouseY();
    ellipse(mx, my, CIRCLE_SIZE, CIRCLE_SIZE);

    graphicsBegin(&pg);
        background(51, 51, 51);
        noFill();
        stroke(P5_WHITE);
        ellipse(mx - 150, my - 75, 60, 60);
    graphicsEnd();

    render(pg, 150, 75, pg.width, pg.height);
}


