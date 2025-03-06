
#include <p5.h>

typedef uint32_t Color;

typedef struct {
    int x, y;
    Color color;
} Point;

typedef struct Points {
    Point point;
    struct Points *prev;
    struct Points *next;
} Points;

Points *points = NULL;

void push_point(int x, int y, Color c) {
    Point p = {
        .x = x,
        .y = y,
        .color = c
    };

    if (points == NULL) {
        points = alloc(sizeof(*points));
        points->point = p;
        points->prev = NULL;
        points->next = NULL;
    } else {
        Points *new_node = alloc(sizeof(*new_node));
        memset(new_node, 0, sizeof(*new_node));
        new_node->point = p;
        new_node->next = points;
        points->prev = new_node;
        points = new_node;
    }
}

const Color colors[] = {
    color(0xff, 0xcc, 0x00, 0xff),
    color(0xff, 0xcc, 0x00, 0xff),
    color(0x00, 0x00, 0xff, 0xff),
    color(0x44, 0x99, 0xff, 0xff),
    color(0x7f, 0xff, 0xf5, 0xff),
    color(0x18, 0x18, 0x18, 0xff),
    color(0x69, 0x69, 0x69, 0xff),
    color(0x33, 0xff, 0x99, 0xff),
    color(0xdd, 0xdd, 0xdd, 0xff),
    color(0x42, 0x42, 0x42, 0xff)
};
#define COLOR_COUNT (sizeof(colors)/sizeof(*colors))

void mousePressed(void *_) {
    push_point(mouseX(), mouseY(), colors[random(0, COLOR_COUNT)]);
}

#define WIDTH  400
#define HEIGHT 400
void setup(void) {
    p5_Element canvas = createCanvas(WIDTH, HEIGHT);
    for (int i = 0; i < 10; i++) {
        push_point(random(0, WIDTH), random(0, HEIGHT), colors[i % COLOR_COUNT]);
    }
    pixelDensity(1);

    onMousePressed(&canvas, mousePressed, NULL);
}

int distSq(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void draw(void) {
    background(P5_BLACK);

    static Color pixels[WIDTH * HEIGHT] = {0};
    loadPixels(pixels);
    for (int i = 0; i < WIDTH * HEIGHT; i ++) {
        const int y = i / WIDTH;
        const int x = i - y * WIDTH;
        
        Point curr_point = {.x = x, .y = y};

        Point closest = points->point;
        for (Points *node = points->next; node != NULL; node = node->next) {
            Point p = node->point;
            if (distSq(curr_point, p) < distSq(curr_point, closest)) {
                closest = p;
            }
        }

        pixels[i] = closest.color;
    }
    updatePixels(pixels);
    
    for (Points *node = points; node != NULL; node = node->next) {
        Point p = node->point;
        strokeWeight(8);
        stroke(P5_WHITE);
        point(p.x, p.y);
    }
}


