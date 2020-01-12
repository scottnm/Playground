use std::fs::File;
use std::io::Write;

#[derive(Clone, Copy, Debug, PartialEq)]
struct Color {
    r: u8,
    g: u8,
    b: u8,
}

struct PPMBuffer {
    width: usize,
    height: usize,
    pixels: Box<[Color]>,
}

impl PPMBuffer {
    fn new(width: usize, height: usize) -> PPMBuffer {
        PPMBuffer {
            width,
            height,
            pixels: vec![Color { r: 0, g: 0, b: 0 }; width * height].into_boxed_slice(),
        }
    }

    fn buffer_index(&self, row: usize, col: usize) -> usize {
        return self.width * row + col;
    }

    fn set(&mut self, row: usize, col: usize, color: Color) {
        let index = self.buffer_index(row, col);
        self.pixels[index] = color;
    }

    fn get(&self, row: usize, col: usize) -> Color {
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
                let pixel: Color = self.get(row, col);
                file.write(format!("{} {} {}\n", pixel.r, pixel.g, pixel.b).as_bytes())
                    .unwrap_or_else(|_| {
                        panic!("Failed to write pixel ({},{}) to {}", row, col, filename)
                    });
            }
        }
    }
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
    for row in (0..ppm_buffer.height).rev() {
        for col in 0..ppm_buffer.width {
            fn to_u8_color(percent: f32) -> u8 {
                return (percent * 255.99) as u8;
            }

            let r = to_u8_color(col as f32 / WIDTH as f32);
            let g = to_u8_color(row as f32 / HEIGHT as f32);
            let b = to_u8_color(0.2);

            ppm_buffer.set(row, col, Color { r, g, b });
        }
    }

    ppm_buffer.write_to_file(filename);
}
