//#![allow(subpar_naming_conventions)]
#![allow(non_snake_case)]

#[repr(C)]
pub struct Element {
    _id: u8,
}
#[repr(C)]
pub struct Image {
    pub loaded: bool,
    _id: u8,
}
#[repr(C)]
pub struct Graphics {
    pub width: i32,
    pub height: i32,
    _id: i32,
}

mod wasm_panic {
    use core::fmt::Write;
    use core::panic::PanicInfo;
    extern "C" {
        fn _wasm_panic(msg: *const u8) -> !;
    }

    struct SliceWriter<'a> {
        buf: &'a mut [u8],
        written: usize,
    }

    impl<'a> Write for SliceWriter<'a> {
        fn write_str(&mut self, s: &str) -> core::fmt::Result {
            let bs = s.as_bytes();
            if self.buf.len() < bs.len() + self.written {
                Err(core::fmt::Error)
            } else {
                for i in 0..bs.len() {
                    self.buf[self.written + i] = bs[i];
                }
                self.written += bs.len();
                Ok(())
            }
        }
    }

    const PANIC_BUF_CAP: usize = 256;
    #[allow(static_mut_refs)]
    #[panic_handler]
    unsafe fn panic<'a, 'b>(info: &'a PanicInfo<'b>) -> ! {
        static mut buf: [u8; PANIC_BUF_CAP] = [0; PANIC_BUF_CAP];
        let mut writer = SliceWriter {
            buf: &mut buf[..],
            written: 0,
        };
        match write!(writer, "{info}\0") {
            Ok(_) => _wasm_panic(writer.buf.as_ptr()),
            Err(_) => _wasm_panic(b"<panic message overflow>\0".as_ptr()),
        }
    }
}

mod c {
    use p5::{Image, Element, Graphics};
    extern "C" {
        // MEMORY MANAGEMENT
        // Reset the heap
        pub fn heapReset();

        // CORE
        pub fn setFrameRate(fps: i32);
        pub fn random(min: i32, max: i32) -> i32;

        // INPUT
        pub fn mouseX() -> i32;
        pub fn mouseY() -> i32;

        // DRAWING
        pub fn background(r: u8, g: u8, b: u8);
        pub fn fill(r: u8, g: u8, b: u8);
        pub fn fillA(r: u8, g: u8, b: u8, alpha: u8);
        pub fn stroke(r: u8, g: u8, b: u8);
        pub fn strokeWeight(weight: i32);
        pub fn circle(x: i32, y: i32, d: i32);
        pub fn ellipse(x: i32, y: i32, w: i32, h: i32);
        pub fn point(x: i32, y: i32);
        pub fn line(x1: i32, y1: i32, x2: i32, y2: i32);
        pub fn rect(x: i32, y: i32, w: i32, h: i32);
        pub fn noStroke();
        pub fn noFill();

        pub fn rotate(angle: f32);
        pub fn translate(dx: i32, dy: i32);
        pub fn push();
        pub fn pop();

        // IMAGES
        // TODO: loadImageSync() that does the same as loadImage() but blocking
        pub fn loadImage(url: *const u8, img: *mut Image);
        pub fn image(img: *const Image, x: i32, y: i32, w: i32, h: i32);

        // LOGGING
        pub fn print(value: i32);
        pub fn print_str(msg: *const u8);

        // DOM
        pub fn createCanvas(width: i32, height: i32) -> Element;
        pub fn createButton(label: *const u8) -> Element;
        pub fn createP(label: *const u8) -> Element;
//        pub fn onMousePressed(elt: *mut Element, cb: extern fn(*mut ()) -> (), data: *mut ());
        pub fn setHTML(elt: *mut Element, data: *const u8);

        // GRAPHICS
        pub fn createGraphics(width: i32, height: i32) -> Graphics;
        pub fn graphicsBegin(gr: *mut Graphics);
        pub fn graphicsEnd();
        pub fn render(gr: *const Graphics, x: i32, y: i32, w: i32, h: i32);
    }
}

fn validate_cstr(cstr: &[u8]) {
    if cstr.last() != Some(&0) {
        panic!("invalid c-string");
    }
}

pub unsafe fn heapReset() {
    c::heapReset()
}

// CORE
pub fn setFrameRate(fps: i32) {
    unsafe { c::setFrameRate(fps) }
}
pub fn random(min: i32, max: i32) -> i32 {
    unsafe { c::random(min, max) }
}

// INPUT
pub fn mouseX() -> i32 {
    unsafe { c::mouseX() }
}
pub fn mouseY() -> i32 {
    unsafe { c::mouseY() }
}

// DRAWING
pub fn background(r: u8, g: u8, b: u8) {
    unsafe { c::background(r, g, b) }
}
pub fn fill(r: u8, g: u8, b: u8) {
    unsafe { c::fill(r, g, b) }
}
pub fn fillA(r: u8, g: u8, b: u8, alpha: u8) {
    unsafe { c::fillA(r, g, b, alpha) }
}
pub fn stroke(r: u8, g: u8, b: u8) {
    unsafe { c::stroke(r, g, b) }
}
pub fn strokeWeight(weight: i32) {
    unsafe { c::strokeWeight(weight) }
}
pub fn circle(x: i32, y: i32, d: i32) {
    unsafe { c::circle(x, y, d) }
}
pub fn ellipse(x: i32, y: i32, w: i32, h: i32) {
    unsafe { c::ellipse(x, y, w, h) }
}
pub fn point(x: i32, y: i32) {
    unsafe { c::point(x, y) }
}
pub fn line(x1: i32, y1: i32, x2: i32, y2: i32) {
    unsafe { c::line(x1, y1, x2, y2) }
}
pub fn rect(x: i32, y: i32, w: i32, h: i32) {
    unsafe { c::rect(x, y, w, h) }
}
pub fn noStroke() {
    unsafe { c::noStroke() }
}
pub fn noFill() {
    unsafe { c::noFill() }
}

pub fn rotate(angle: f32) {
    unsafe { c::rotate(angle) }
}
pub fn translate(dx: i32, dy: i32) {
    unsafe { c::translate(dx, dy) }
}
pub fn push() {
    unsafe { c::push() }
}
pub fn pop() {
    unsafe { c::pop() }
}

// IMAGES
// TODO: loadImageSync() that does the same as loadImage() but blocking
pub fn loadImage(url: &[u8], img: &mut Image) {
    validate_cstr(url);
    unsafe { c::loadImage(url.as_ptr(), img as *mut _) };
}
pub fn image(img: &Image, x: i32, y: i32, w: i32, h: i32) {
    unsafe { c::image(img as *const _, x, y, w, h) }
}

// LOGGING
pub fn print(value: i32) {
    unsafe { c::print(value) }
}
pub fn print_str(msg: &[u8]) {
    validate_cstr(msg);
    unsafe { c::print_str(msg.as_ptr()) };
}

// DOM
pub fn createCanvas(width: i32, height: i32) -> Element {
    unsafe { c::createCanvas(width, height) }
}
pub fn createButton(label: &[u8]) -> Element {
    validate_cstr(label);
    unsafe { c::createButton(label.as_ptr()) }
}
pub fn createP(label: &[u8]) -> Element {
    validate_cstr(label);
    unsafe { c::createP(label.as_ptr()) }
}
// NOTE: rustc linker sucks so no callbacks for us
//        pub fn onMousePressed(elt: *mut Element, cb: fn(*mut ()) -> (), data: *mut ()) {
//            unsafe { c::onMousePressed(elt: *mut Element, cb: fn(*mut ()) -> (), data: *mut ()) }
//        }
pub fn setHTML(elt: &mut Element, data: &[u8]) {
    validate_cstr(data);
    unsafe { c::setHTML(elt as *mut _, data.as_ptr()) }
}

// GRAPHICS
pub fn createGraphics(width: i32, height: i32) -> Graphics {
    unsafe { c::createGraphics(width, height) }
}
// TODO: some smart-ass system using the Drop trait to automagically graphicsEnd()
pub fn graphicsBegin(gr: &mut Graphics) {
    unsafe { c::graphicsBegin(gr as *mut _) }
}
pub fn graphicsEnd() {
    unsafe { c::graphicsEnd() }
}
pub fn render(gr: &Graphics, x: i32, y: i32, w: i32, h: i32) {
    unsafe { c::render(gr as *const _, x, y, w, h) }
}
