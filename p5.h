#ifndef P5_H_
#define P5_H_

#include <stdbool.h>
#include <stdint.h>
#define NULL ((void*)0)

typedef uint8_t u8;

#define P5_SIG_COLOR u8 r, u8 g, u8 b

#define P5_WHITE 255, 255, 255
#define P5_BLACK 0, 0, 0
#define PI 3.1415926f

// MATH
float sqrtf(float x);

// MEMORY MANAGEMENT
// Reset the heap
void heapReset(void);

// CORE
void setFrameRate(int fps);
int random(int min, int max);

// INPUT
int mouseX(void);
int mouseY(void);

// DRAWING
void background(P5_SIG_COLOR);
void fill(P5_SIG_COLOR);
void fillA(P5_SIG_COLOR, u8 alpha);
void stroke(P5_SIG_COLOR);
void strokeWeight(int weight);
void circle(int x, int y, int d);
void ellipse(int x, int y, int w, int h);
void point(int x, int y);
void line(int x1, int y1, int x2, int y2);
void rect(int x, int y, int w, int h);
void noStroke(void);
void noFill(void);

void rotate(float angle);
void translate(int dx, int dy);
void push(void);
void pop(void);

// PIXELS
// AABBGGRR
#define color(r, g, b, a) ( \
          ((r)&0xFF)<<(8*0) \
        | ((g)&0xFF)<<(8*1) \
        | ((b)&0xFF)<<(8*2) \
        | ((a)&0xFF)<<(8*3) )
void pixelDensity(int density);
void loadPixels(uint32_t *pixels);
void updatePixels(const uint32_t *pixels);


// 3D
void camera1(int x, int y, int z);
void perspective(float fovy, int aspect, float near, float far);
void rotateX(float angle);
void rotateY(float angle);
void box(int w, int h, int d);

// SOUND
typedef struct {
    bool loaded;
    u8 _id;
} p5_Sound;
void loadSound(const char *url, p5_Sound *out);
void playSound(p5_Sound *sound);

// IMAGES
typedef struct {
    bool loaded;
    // We can't directly store images in WASM memory, so we store
    // an id (similar to a file descriptor in UNIX) that represents it.
    u8 _id;
} p5_Image;
// TODO: loadImageSync() that does the same as loadImage() but blocking
void loadImage(const char *url, p5_Image *img);
void image(const p5_Image *img, int x, int y, int w, int h);

// LOGGING
void print(int value);
void print_str(const char *msg);

// DOM
typedef struct {
    u8 _id;
} p5_Element;

// Creates a canvas with the given width and height
// Returns: the canvas
p5_Element createCanvas(int width, int height);
// Creates a canvas in WEBGL mode with the given width and height
// Returns: the canvas
p5_Element createCanvas3d(int width, int height);
// Creates a button with the given label
// Returns: the button
p5_Element createButton(const char *label);
// Creates a <p> with the given label
// Returns: the <p>
p5_Element createP(const char *label);
// Call `cb` whenever the mouse is pressed on `elt`
// `data` is passed to the callback
void onMousePressed(p5_Element *elt, void (*cb)(void*), void *data);
// Sets innerHTML of an element
void setHTML(p5_Element *elt, const char *data);

// GRAPHICS
typedef struct {
    int width;
    int height;
    u8 _id;
} p5_Graphics;
p5_Graphics createGraphics(int width, int height);
void graphicsBegin(p5_Graphics *gr);
void graphicsEnd();
void render(p5_Graphics gr, int x, int y, int w, int h);
void copy(p5_Graphics *gr, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);

#endif // P5_H_

