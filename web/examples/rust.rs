#![no_main]
#![no_std]
//#![allow(normal_naming_conventions)]
#![allow(non_upper_case_globals)]

#[path="../../p5.rs"]
pub mod p5;

const WIDTH:  i32 = 400;
const HEIGHT: i32 = 400;

#[no_mangle]
pub fn setup() {
    p5::createCanvas(WIDTH, HEIGHT);
}

#[derive(Clone, Copy)]
struct Rect {
    x: i32, y: i32,
    w: i32, h: i32,
    r: u8, g: u8, b: u8,
    velx: i32, vely: i32,
}

impl Rect {
    fn random() -> Self {
        let x = p5::random(0, WIDTH);
        let y = p5::random(0, HEIGHT);
        let w = p5::random(0, WIDTH - x); 
        let h = p5::random(0, HEIGHT - y);
        let r = p5::random(0, u8::MAX.into()) as u8;
        let g = p5::random(0, u8::MAX.into()) as u8;
        let b = p5::random(0, u8::MAX.into()) as u8;
        let velx = p5::random(0, 2) * 2 - 1;
        let vely = p5::random(0, 2) * 2 - 1;
        Self { x, y, w, h, r, g, b, velx, vely }
    }

    const EMPTY: Self = Self { x: 0, y: 0, w: 0, h: 0, r: 0, g: 0, b: 0, velx: 0, vely: 0 };
}
const RECT_CAP: usize = 200;
static mut rects: [Rect; RECT_CAP] = [Rect::EMPTY; RECT_CAP];
static mut rect_count: usize = 0;

static mut frame_count: i32 = 0;

#[no_mangle]
pub unsafe fn draw() {
    p5::background(255, 255, 255);

    for i in 0..rect_count {
        let rect = &mut rects[i];

        rect.x += rect.velx;
        rect.y += rect.vely;

        p5::fill(rect.r, rect.g, rect.b);
        p5::noStroke();
        p5::rect(rect.x, rect.y, rect.w, rect.h);
    }

    if rect_count < RECT_CAP && frame_count % 2 == 0 {
        rects[rect_count] = Rect::random();
        rect_count += 1;
    } else if rect_count >= RECT_CAP {
        panic!("rect overflow");
    }

    frame_count += 1;
}
