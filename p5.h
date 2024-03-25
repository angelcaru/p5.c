#ifndef P5_H_
#define P5_H_

#include <stdbool.h>
#define NULL ((void*)0)

typedef unsigned char u8;

#define P5_SIG_COLOR u8 r, u8 g, u8 b

#define P5_WHITE 255, 255, 255
#define P5_BLACK 0, 0, 0

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
void rect(int x, int y, int w, int h);
void noStroke(void);
void noFill(void);

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
// Creates a button with the given label
// Returns: the button
p5_Element createButton(const char *label);
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

#endif // P5_H_

