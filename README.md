# p5.c
WASM interface for [p5.js](https://p5js.org) designed to be used from C.

## Quick Start
```console
$ make
$ cd web
$ python3 -m http.server 6969 &
$ iexplore.exe localhost:6969/?dvd.wasm
```

## Making Your Own Program
Simply write some boilerplate like this:
```c
#include <p5.h>

#define WIDTH  400
#define HEIGHT 400
void setup(void) {
    createCanvas(WIDTH, HEIGHT);
}

void draw(void) {
    background(P5_BLACK);
}
```

and compile with the same command as seen in the [Makefile](./Makefile). To run it, change the URL
to `http://localhost:6969/?your_program_name.wasm`.

