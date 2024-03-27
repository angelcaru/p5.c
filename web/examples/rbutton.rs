#![no_main]
#![no_std]
//#![allow(normal_naming_conventions)]
#![allow(non_upper_case_globals)]

#[path = "../../p5.rs"]
pub mod p5;

#[no_mangle]
pub unsafe fn setup() {
    p5::createCanvas(400, 400);

    p5::createButton(b"Change color!\0").mousePressed(|| {
        r = p5::random(0, 255) as u8;
        g = p5::random(0, 255) as u8;
        b = p5::random(0, 255) as u8;
    });
}

static mut r: u8 = 0;
static mut g: u8 = 0;
static mut b: u8 = 0;

#[no_mangle]
pub unsafe fn draw() {
    p5::background(r, g, b);
}
