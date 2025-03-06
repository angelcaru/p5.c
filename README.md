# p5.c
WASM interface for [p5.js](https://p5js.org) designed to be used from C.

## Quick Start
```console
$ make
$ python3 -m http.server 6969 &
$ iexplore.exe localhost:6969/?dvd.wasm
```

## Making Your Own Program
See [standalone.html](./web/standalone.html), as well as [Makefile](./Makefile) for the compile command (it's quite long).
The C code you need is something like this:
```c
#include <p5.h>

#define WIDTH 640
#define HEIGHT 480
void setup(void) {
    createCanvas(WIDTH, HEIGHT);
}

void draw(void) {
    background(0x18, 0x18, 0x18);
}
```


