mod color;
mod math;

use crate::color::*;
use crate::math::*;

use std::fs::File;
use std::io::Write;
use std::option::Option;

extern crate rand;
use rand::Rng;
use rand::SeedableRng;
use rand::rngs::StdRng;

fn random_deviation<T: Rng>(random_generator: &mut T, factor: f32) -> f32 {
    let sign = if random_generator.gen_bool(0.5) { 1.0 } else { -1.0 };
    sign * factor * random_generator.gen::<f32>()
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
    let repeat_factor = 3.0; // a multiplier to make the rainbow repeat faster
    let h_unbounded = t * 360.0 * repeat_factor;

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
        let hit = ray_hit_sphere(&ray, sphere);
        if hit.is_some() {
            return hit;
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

fn seed_from_string(s: &str) -> [u8; 32] {
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
    let mut rng = StdRng::from_seed(seed_from_string(filename));

    // Fill the PPM Buffer
    let mut ppm_buffer = PPMBuffer::new(WIDTH, HEIGHT);

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
mod raytracer_tests {
    use super::*;

    #[test]
    fn test_ray_hit_sphere() {
        let ray =
            Ray { origin: Vec3 { x: 0.0, y: 0.0, z: 0.0 }, dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 10.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 1.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere1).is_some());
        assert!(ray_hit_sphere(&ray, &sphere2).is_some());
    }

    #[test]
    fn test_ray_miss_sphere() {
        let ray =
            Ray { origin: Vec3 { x: 0.0, y: 0.0, z: 0.0 }, dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 11.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 0.0, y: 200.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere1).is_none());
        assert!(ray_hit_sphere(&ray, &sphere2).is_none());
    }
}
