use std::fmt;
use std::fs::File;
use std::io::Write;
use std::ops::{Add, Div, Mul, Sub};
use std::option::Option;

// FIXME: move into its own abstracted helper file (I don't love the bleed out that rand
// dependencies have)
extern crate rand;
use rand::Rng;

fn random_deviation<T: Rng>(random_generator: &mut T, factor: f32) -> f32 {
    let sign = if random_generator.gen_bool(0.5) { 1.0 } else { -1.0 };
    sign * factor * random_generator.gen::<f32>()
}

extern crate num; // used for rgb casting

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

impl<T: Div<Output = T> + Copy> Div<T> for Rgb<T> {
    type Output = Self;
    fn div(self, rhs: T) -> Self::Output {
        Self { r: self.r / rhs, g: self.g / rhs, b: self.b / rhs }
    }
}

impl<T: Add<Output = T>> Add for Rgb<T> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        Self { r: self.r + rhs.r, g: self.g + rhs.g, b: self.b + rhs.b }
    }
}

fn rgb_cast<FT: num::NumCast, TT: num::NumCast>(from: Rgb<FT>) -> Rgb<TT> {
    Rgb {
        r: num::cast(from.r).unwrap(),
        g: num::cast(from.g).unwrap(),
        b: num::cast(from.b).unwrap(),
    }
}

fn ratio_to_rgb(rgbf: Rgb<f32>) -> Rgb<u8> {
    const ALMOST_MAX: f32 = (std::u8::MAX as f32) - 0.001;
    rgb_cast(rgbf * ALMOST_MAX)
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

fn raycast_bg_blue(r: &Ray) -> Rgb<u8> {
    let unit_direction = unit_vector(r.dir);
    let t = 0.5 * (unit_direction.y + 1.0);
    let start_white = Rgb { r: 1.0, g: 1.0, b: 1.0 };
    let end_blue = Rgb { r: 0.5, g: 0.7, b: 1.0 };
    let lerped_blue = start_white * (1.0 - t) + end_blue * t;

    ratio_to_rgb(lerped_blue)
}

fn raycast_bg_rainbow(r: &Ray) -> Rgb<u8> {
    let unit_ray = unit_vector(r.dir);
    let t = (unit_ray.y + 1.0) * 0.5;
    let h_unbounded = t * 360.0;

    // shift the hue to make the middle of the rainbow standout against the blue background
    let h_unbounded = h_unbounded + 200.0;
    let h = (h_unbounded as u16) % 360;

    hsv_to_rgb(Hsv { h, s: 0.8, v: 0.8 })
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
        // TODO: add a get_row that returns a row span to avoid some unnecessary
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
        dot(*self, *self).sqrt()
    }
}

impl fmt::Display for Vec3 {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "({}, {}, {})", self.x, self.y, self.z)
    }
}

fn dot(v1: Vec3, v2: Vec3) -> f32 {
    (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)
}

impl Add<Vec3> for Vec3 {
    type Output = Vec3;
    fn add(self, rhs: Vec3) -> Vec3 {
        Vec3 { x: self.x + rhs.x, y: self.y + rhs.y, z: self.z + rhs.z }
    }
}

impl Sub<Vec3> for Vec3 {
    type Output = Vec3;
    fn sub(self, rhs: Vec3) -> Vec3 {
        Vec3 { x: self.x - rhs.x, y: self.y - rhs.y, z: self.z - rhs.z }
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

// A ray is described as a starting point, the origin, and a direction to travel. Points along the
// ray are parameterized as P(t) = origin + (dir * t).
struct Ray {
    origin: Vec3,
    dir: Vec3,
}

struct Sphere {
    center: Vec3,
    radius: f32,
}

struct HitRecord {
    point: Vec3,
    normal: Vec3,
}

fn point_on_ray(r: &Ray, t: f32) -> Vec3 {
    r.origin + r.dir * t
}

fn ray_hit_sphere(r: &Ray, s: &Sphere) -> Option<HitRecord> {
    // The points along the surface of the sphere are described as...
    //    (X-Cx)^2 + (Y - Cy)^2 + (Z-Cz)^2 = R^2
    // In english, "Any point at a distance of exactly the radius is on the surface of the sphere"
    //
    // If we are interested in understanding whether a ray intersects with the surface of the
    // sphere we can use our parameterized ray function to solve!
    //
    //    (P(t).x-Cx)^2 + (P(t).y-Cy)^2 + (P(Z).z-Cz)^2 = R^2
    //    dot(P(t)-C, P(t)-C) = R^2
    //    A = RayOrigin, B = RayDir, C = SphereCenter
    //    dot(A+B*t-C, A+B*t-C) = R^2
    //    t^2 * dot(B,B) + 2t * dot(B,A-C) + dot(A-C,A-C) - R^2 = 0
    //
    // Now we have a quadratic equation!
    //
    // The quadratic formula...
    //     x = (-b +/- sqrt(b^2 - 4ac)) / 2a
    // ... can be used to solve for solutions to quadratic functions. In general, since we only
    // care about knowing whether something hit (and not where) we only care about the sqrt portion
    // of the formula.
    // - When the sqrt root operand is positive there are 2 solutions
    // - When the sqrt root operand is zero there is 1 solution
    // - When the sqrt root operand is negative there are no solutions
    let a = dot(r.dir, r.dir);
    let b = 2.0 * dot(r.dir, r.origin - s.center);
    let c = dot(r.origin - s.center, r.origin - s.center) - (s.radius * s.radius);
    let discriminant = b * b - 4.0 * a * c;

    if discriminant >= 0.0 {
        // FIXME: guard against negative t values
        let hit_t = (-b - discriminant.sqrt()) / (2.0 * a);
        let point = point_on_ray(&r, hit_t);
        let normal = point - s.center;
        Some(HitRecord { point, normal: unit_vector(normal) })
    } else {
        None
    }
}

fn cast_ray(ray: &Ray) -> Option<HitRecord> {
    // TODO: enable not hardcoding the list of spheres/surfaces
    const SPHERES: [Sphere; 4] = [
        Sphere { center: Vec3 { x: 0.0, y: 0.0, z: -50.0 }, radius: 5.0 },
        Sphere { center: Vec3 { x: -25.0, y: 0.0, z: -50.0 }, radius: 10.0 },
        Sphere { center: Vec3 { x: 0.0, y: 25.0, z: -50.0 }, radius: 5.0 },
        Sphere { center: Vec3 { x: 30.0, y: -10.0, z: -50.0 }, radius: 15.0 },
    ];

    for sphere in &SPHERES {
        if let Some(hit) = ray_hit_sphere(&ray, sphere) {
            return Some(hit);
        }
    }

    None
}

fn ray_color(ray: &Ray) -> Rgb<u8> {
    match cast_ray(&ray) {
        // Hit a sphere!
        // EFFECT: left side of spheres are rainbow gradients; right side of spheres are normals visualization
        // TODO: support translucent spheres
        Some(hit) => {
            if hit.normal.x > 0.0 {
                let norm_as_color_ratio = Rgb { r: hit.normal.x, g: hit.normal.y, b: hit.normal.z };
                let color_ratio_normalized =
                    (norm_as_color_ratio + Rgb { r: 1.0, g: 1.0, b: 1.0 }) * 0.5;
                ratio_to_rgb(color_ratio_normalized)
            } else {
                raycast_bg_rainbow(ray)
            }
        }

        // Hit no surfaces, render a background
        None => raycast_bg_blue(ray),
    }
}

fn string_to_randseed(s: &str) -> [u8; 32] {
    let mut seed = [0; 32];
    for i in 0..seed.len() {
        seed[i] = if s.len() > i { s.as_bytes()[i] } else { 0 };
    }
    seed
}

fn main() {
    const WIDTH: usize = 400;
    const HEIGHT: usize = 200;
    const BG_DEPTH: f32 = -100.0;

    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        println!("Usage: raytrace_rs.exe [filename]");
        return;
    }

    let filename: &str = &args[1];

    // Fill the PPM Buffer
    let mut ppm_buffer = PPMBuffer::new(WIDTH, HEIGHT);
    let mut rng: rand::rngs::StdRng = rand::SeedableRng::from_seed(string_to_randseed(filename));

    // Render world
    for row in (0..ppm_buffer.height).rev() {
        for col in 0..ppm_buffer.width {
            let mut sample_sum = Rgb::<u64> { r: 0, g: 0, b: 0 };

            const AA_SAMPLE_SIZE: u64 = 8;
            for _s in 0..AA_SAMPLE_SIZE {
                const EYE: Vec3 = Vec3 { x: 0.0, y: 0.0, z: 0.0 };
                let sample_col = col as f32 + random_deviation(&mut rng, 0.5);
                let sample_row = row as f32 + random_deviation(&mut rng, 0.5);
                let look_dir = Vec3 {
                    x: sample_col - ppm_buffer.width as f32 / 2.0,
                    y: sample_row - ppm_buffer.height as f32 / 2.0,
                    z: BG_DEPTH,
                };

                let look_ray = Ray { origin: EYE, dir: look_dir };
                sample_sum = sample_sum + rgb_cast(ray_color(&look_ray));
            }

            let sampled_color = rgb_cast(sample_sum / AA_SAMPLE_SIZE);
            ppm_buffer.set(row, col, sampled_color);
        }
    }

    ppm_buffer.write_to_file(filename);
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

    #[test]
    fn test_ray_hit_sphere() {
        let ray =
            Ray { origin: Vec3 { x: 0.0, y: 0.0, z: 0.0 }, dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 10.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 1.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere1));
        assert!(ray_hit_sphere(&ray, &sphere2));
    }

    #[test]
    fn test_ray_miss_sphere() {
        let ray =
            Ray { origin: Vec3 { x: 0.0, y: 0.0, z: 0.0 }, dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 11.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 0.0, y: 200.0, z: 0.0 }, radius: 10.0 };
        assert!(!ray_hit_sphere(&ray, &sphere1));
        assert!(!ray_hit_sphere(&ray, &sphere2));
    }
}
