use std::fs::File;
use std::io::Write;
use std::ops::{Add, Div, Mul};

#[derive(Clone, Copy, Debug, PartialEq)]
struct Rgb<T> {
    r: T,
    g: T,
    b: T,
}

impl<T: Mul<Output = T> + Copy> Mul<T> for Rgb<T> {
    type Output = Self;
    fn mul(self, rhs: T) -> Self::Output {
        Self { r: self.r * rhs, g: self.g * rhs, b: self.b * rhs }
    }
}

impl<T: Add<Output = T>> Add for Rgb<T> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        Self { r: self.r + rhs.r, g: self.g + rhs.g, b: self.b + rhs.b }
    }
}

fn rgb_f32_to_u8(rgbf: Rgb<f32>) -> Rgb<u8> {
    Rgb { r: (rgbf.r * 255.99) as u8, g: (rgbf.g * 255.99) as u8, b: (rgbf.b * 255.99) as u8 }
}

#[derive(Clone, Copy, Debug, PartialEq)]
struct Hsv {
    h: u16, // [0, 360)
    s: f32,
    v: f32,
}

fn hsv_to_rgb(color: Hsv) -> Rgb<u8> {
    let c: f32 = color.s * color.v;
    let x: f32 = c * (1.0 - (((color.h as f32 / 60.0) % 2.0) - 1.0).abs()) as f32;
    let m: f32 = color.v - c;

    let h = color.h % 360; // just wrap my h values to keep things simple
    let prime = match h {
        0..=59 => (c, x, 0.0),
        60..=119 => (x, c, 0.0),
        120..=179 => (0.0, c, x),
        180..=239 => (0.0, x, c),
        240..=299 => (x, 0.0, c),
        _ => (c, 0.0, x),
    };

    let prime_to_rgb = |v: f32| -> u8 { ((v + m) * 255.0) as u8 };

    Rgb { r: prime_to_rgb(prime.0), g: prime_to_rgb(prime.1), b: prime_to_rgb(prime.2) }
}

fn raycast_bg_blue(r: Ray) -> Rgb<u8> {
    let unit_direction = unit_vector(r.dir);
    let t = 0.5 * (unit_direction.y + 1.0);
    let start_white = Rgb { r: 1.0, g: 1.0, b: 1.0 };
    let end_blue = Rgb { r: 0.5, g: 0.7, b: 1.0 };
    let lerped_blue = start_white * (1.0 - t) + end_blue * t;

    rgb_f32_to_u8(lerped_blue)
}

fn raycast_bg_rainbow(r: Ray) -> Rgb<u8> {
    let unit_ray = unit_vector(r.dir);
    let t = (unit_ray.y + 1.0) * 0.5;
    let h = t * 360.0;

    hsv_to_rgb(Hsv { h: h as u16, s: 0.5, v: 0.75 })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hsv_to_rgb_1() {
        let hsv = Hsv { h: 355, s: 0.5, v: 0.75 };
        let rgb = Rgb { r: 191, g: 95, b: 103 };
        assert_eq!(hsv_to_rgb(hsv), rgb);
    }

    #[test]
    fn test_hsv_to_rgb_2() {
        let hsv = Hsv { h: 240, s: 0.1, v: 0.99 };
        let rgb = Rgb { r: 227, g: 227, b: 252 };
        assert_eq!(hsv_to_rgb(hsv), rgb);
    }

    #[test]
    fn test_hsv_to_rgb_3() {
        let hsv = Hsv { h: 240, s: 0.99, v: 0.1 };
        let rgb = Rgb { r: 0, g: 0, b: 25 };
        assert_eq!(hsv_to_rgb(hsv), rgb);
    }
}

struct PPMBuffer {
    width: usize,
    height: usize,
    pixels: Box<[Rgb<u8>]>,
}

impl PPMBuffer {
    fn new(width: usize, height: usize) -> PPMBuffer {
        PPMBuffer {
            width,
            height,
            pixels: vec![Rgb { r: 0, g: 0, b: 0 }; width * height].into_boxed_slice(),
        }
    }

    fn buffer_index(&self, row: usize, col: usize) -> usize {
        return self.width * row + col;
    }

    fn set(&mut self, row: usize, col: usize, color: Rgb<u8>) {
        let index = self.buffer_index(row, col);
        self.pixels[index] = color;
    }

    fn get(&self, row: usize, col: usize) -> Rgb<u8> {
        self.pixels[self.buffer_index(row, col)]
    }

    fn write_to_file(&self, filename: &str) {
        let file_result: Result<File, std::io::Error> = File::create(filename);
        let mut file: File = file_result.unwrap_or_else(|_| panic!("Failed to open {}", filename));

        // Write the PPM Header
        let header: String = format!("P3\n{} {}\n255\n", self.width, self.height);
        file.write(header.as_bytes())
            .unwrap_or_else(|_| panic!("Failed to write PPM header to {}", filename));

        // Write the PPM body
        // @Improvement: add a get_row that returns a row span to avoid some unnecessary
        // multiplications for each `self.get` call
        for row in (0..self.height).rev() {
            for col in 0..self.width {
                let pixel: Rgb<u8> = self.get(row, col);
                file.write(format!("{} {} {}\n", pixel.r, pixel.g, pixel.b).as_bytes())
                    .unwrap_or_else(|_| {
                        panic!("Failed to write pixel ({},{}) to {}", row, col, filename)
                    });
            }
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq)]
struct Vec3 {
    x: f32,
    y: f32,
    z: f32,
}

impl Vec3 {
    fn mag(&self) -> f32 {
        let square_comp_sum = (self.x * self.x) + (self.y * self.y) + (self.z * self.z);
        square_comp_sum.sqrt()
    }
}

impl Add<Vec3> for Vec3 {
    type Output = Vec3;
    fn add(self, rhs: Vec3) -> Vec3 {
        Vec3 { x: self.x + rhs.x, y: self.y + rhs.y, z: self.z + rhs.z }
    }
}

impl Mul<f32> for Vec3 {
    type Output = Vec3;
    fn mul(self, rhs: f32) -> Vec3 {
        Vec3 { x: self.x * rhs, y: self.y * rhs, z: self.z * rhs }
    }
}

impl Div<f32> for Vec3 {
    type Output = Vec3;
    fn div(self, rhs: f32) -> Vec3 {
        Vec3 { x: self.x / rhs, y: self.y / rhs, z: self.z / rhs }
    }
}

fn unit_vector(v: Vec3) -> Vec3 {
    v / v.mag()
}

struct Ray {
    origin: Vec3,
    dir: Vec3,
}

fn main() {
    const WIDTH: usize = 200;
    const HEIGHT: usize = 100;

    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        println!("Usage: pathtrace_rs.exe [filename]");
        return;
    }

    let filename: &str = &args[1];

    // Fill the PPM Buffer
    let mut ppm_buffer = PPMBuffer::new(WIDTH, HEIGHT);

    // Render the background
    let eye = Vec3 { x: 0.0, y: 0.0, z: 0.0 };
    for row in (0..ppm_buffer.height).rev() {
        for col in 0..ppm_buffer.width {
            // look at each pixel
            let look_dir = Vec3 {
                x: (col as isize - ppm_buffer.width as isize / 2) as f32,
                y: (row as isize - ppm_buffer.height as isize / 2) as f32,
                z: -1.0,
            };
            let look_ray = Ray { origin: eye, dir: look_dir };
            ppm_buffer.set(row, col, raycast_bg_rainbow(look_ray));
        }
    }

    // Render the embedded rainbow gradient
    let inner_rect_height = ppm_buffer.height / 2;
    let inner_rect_width = ppm_buffer.width / 2;
    for row in (0..inner_rect_height).rev() {
        for col in 0..inner_rect_width {
            let color_ratio = Rgb {
                r: col as f32 / inner_rect_width as f32,
                g: row as f32 / inner_rect_height as f32,
                b: 0.2,
            };

            ppm_buffer.set(
                row + inner_rect_height / 2,
                col + inner_rect_width / 2,
                rgb_f32_to_u8(color_ratio),
            );
        }
    }

    ppm_buffer.write_to_file(filename);
}
