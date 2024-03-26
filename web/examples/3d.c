#include <p5.h>

#define SIZE 400
#define HALF_SIZE (SIZE/2)
void setup(void) {
    createCanvas3d(SIZE, SIZE);
    camera1(0, 0, HALF_SIZE * sqrtf(3.0f));
    perspective(PI/3, 1, HALF_SIZE/10*sqrtf(3.0f), HALF_SIZE*10*sqrtf(3.0f));
}

void draw(void) {
    static int frameCount = 0;
    background(200, 200, 200);
    rotateX(frameCount * 0.01f);
    rotateY(frameCount * 0.01f);
    box(HALF_SIZE, HALF_SIZE, HALF_SIZE);

    frameCount++;
}
