mod cmdline;
mod color;
mod math;

extern crate rand;
use crate::color::*;
use crate::math::*;

use std::collections::BTreeMap;
use std::fs::File;
use std::io::Write;
use std::option::Option;

use rand::rngs::StdRng;
use rand::Rng;
use rand::SeedableRng;

fn random_deviation<T: Rng>(rng: &mut T, factor: f32) -> f32 {
    let sign = if rng.gen_bool(0.5) { 1.0 } else { -1.0 };
    sign * factor * rng.gen::<f32>()
}

fn raycast_bg_blue(r: &Ray) -> Rgb<f32> {
    let unit_direction = unit_vector(r.dir);
    let t = 0.5 * (unit_direction.y + 1.0);
    let white = Rgb::all(1.0);
    let blue = Rgb { r: 0.5, g: 0.7, b: 1.0 };
    lerp(white, blue, t)
}

fn raycast_bg_rainbow(r: &Ray) -> Rgb<f32> {
    let unit_ray = unit_vector(r.dir);
    let t = (unit_ray.y + 1.0) * 0.5;
    let repeat_factor = 1.0; // a multiplier to make the rainbow repeat faster
    let h_unbounded = t * 360.0 * repeat_factor;

    // shift the hue to make the middle of the rainbow standout against the blue background
    let h_unbounded = h_unbounded + 200.0;
    let h = (h_unbounded as u16) % 360;

    hsv_to_rgb(Hsv { h, s: 0.8, v: lerp(0.8, 0.5, t) })
}

struct PPMBuffer {
    width: usize,
    height: usize,
    pixels: Box<[Rgb<u8>]>,
}

impl PPMBuffer {
    fn new(width: usize, height: usize) -> PPMBuffer {
        PPMBuffer { width, height, pixels: vec![Rgb::all(0); width * height].into_boxed_slice() }
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

fn write_meta_to_file(options: &cmdline::Opts, profile: &BTreeMap<&str, std::time::Duration>) {
    let filename: &str = &options.meta_output;
    let file_result: Result<File, std::io::Error> = File::create(filename);
    let mut file: File = file_result.unwrap_or_else(|_| panic!("Failed to open {}", filename));

    let mut lines = Vec::new();
    lines.push(format!("{:?}", options));
    lines.push(format!(""));
    lines.push(format!(" Profile    | Time     "));
    lines.push(format!("------------|----------"));
    for (name, time) in profile {
        lines.push(format!(" {:10} | {:6}s", name, time.as_secs()));
    }

    for line in lines {
        let delimited_line = format!("{}\n", line);
        file.write(delimited_line.as_bytes()).expect("Failed to write line");
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
    t: f32,
    point: Vec3,
    normal: Vec3,
}

fn point_on_ray(r: &Ray, t: f32) -> Vec3 {
    r.origin + r.dir * t
}

fn ray_hit_sphere(r: &Ray, s: &Sphere, t_max: f32) -> Option<HitRecord> {
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
        // Quadratic solutions have at most 2 solutions
        let solutions = [
            (-b - discriminant.sqrt()) / (2.0 * a), // did we intersect with the outside of a surface?
            (-b + discriminant.sqrt()) / (2.0 * a), // did we intersect with the inside of a surface?
        ];

        for hit_t in &solutions {
            // Ignore negative t-values to prevent from "hitting" values behind the ray origin
            // Ignore t-values close to 0 to avoid shadow acne
            const T_MIN: f32 = 0.001;
            if *hit_t > T_MIN && *hit_t < t_max {
                let point = point_on_ray(&r, *hit_t);
                let normal = unit_vector(point - s.center);
                return Some(HitRecord { t: *hit_t, point, normal });
            }
        }

        // Both solutions had invalid t-values, ignore solution
        None
    } else {
        // No solution
        None
    }
}

fn cast_ray(ray: &Ray) -> Option<HitRecord> {
    // TODO: enable not hardcoding the list of spheres/surfaces
    const SPHERES: [Sphere; 6] = [
        Sphere { center: Vec3 { x: 0.0, y: 0.0, z: -0.5 }, radius: 0.05 },
        Sphere { center: Vec3 { x: -0.25, y: 0.0, z: -0.5 }, radius: 0.1 },
        Sphere { center: Vec3 { x: 0.0, y: 0.25, z: -0.5 }, radius: 0.05 },
        Sphere { center: Vec3 { x: 0.30, y: -0.10, z: -0.5 }, radius: 0.15 },
        Sphere { center: Vec3 { x: 0.0, y: -100.5, z: -1.0 }, radius: 100.0 },
        Sphere { center: Vec3 { x: 0.0, y: 0.0, z: -1.0 }, radius: 0.5 },
    ];

    let mut t_max = std::f32::MAX;
    let mut closest_hit: Option<HitRecord> = None;
    for sphere in &SPHERES {
        let next_hit = ray_hit_sphere(&ray, sphere, t_max);
        if let Some(hit) = next_hit {
            t_max = hit.t;
            closest_hit = Some(hit);
        }
    }

    closest_hit
}

fn ray_color<T: Rng>(ray: &Ray, max_reflects: usize, rng: &mut T) -> Rgb<f32> {
    match cast_ray(&ray) {
        // Hit a sphere!
        // TODO: support translucent spheres
        Some(hit) => {
            let random_unit_sphere_dir = unit_vector(Vec3 {
                x: random_deviation(rng, 1.0),
                y: random_deviation(rng, 1.0),
                z: random_deviation(rng, 1.0),
            });
            let random_reflect_dir = hit.normal + random_unit_sphere_dir;
            let reflection_ray = Ray { origin: hit.point, dir: random_reflect_dir };

            if max_reflects > 0 {
                ray_color(&reflection_ray, max_reflects - 1, rng) * 0.5
            } else {
                raycast_bg_blue(ray)
            }
        }

        // Hit no surfaces, render a background
        // None => raycast_bg_rainbow(ray),
        None => raycast_bg_blue(ray),
    }
}

fn apply_gamma(color_ratio: Rgb<f32>) -> Rgb<f32> {
    Rgb { r: color_ratio.r.sqrt(), g: color_ratio.g.sqrt(), b: color_ratio.b.sqrt() }
}

fn seed_from_string(s: &str) -> [u8; 32] {
    let mut seed = [0; 32];
    for i in 0..seed.len() {
        seed[i] = if s.len() > i { s.as_bytes()[i] } else { 0 };
    }
    seed
}

struct Camera {
    view_width: f32,
    view_height: f32,
    distance_to_screen: f32,
    origin: Vec3,
}

impl Camera {
    fn new() -> Camera {
        Camera {
            view_width: 4.0,
            view_height: 2.0,
            distance_to_screen: 1.0,
            origin: Vec3::all(0.0),
        }
    }

    fn ray_to_screen(&self, u: f32, v: f32) -> Ray {
        Ray {
            origin: self.origin,
            dir: Vec3 {
                x: (u - 0.5) * self.view_width,
                y: (v - 0.5) * self.view_height,
                z: -self.distance_to_screen,
            },
        }
    }
}

fn main() {
    const MAX_REFLECTS: usize = 6;

    let options = cmdline::Opts::from_args();
    println!("{:?}\n", options);

    let output_filename: &str = &options.output;
    let mut profiled_segments = BTreeMap::new();

    // Setup necessary render state
    let mut rng = StdRng::from_seed(seed_from_string(output_filename));
    let mut ppm_buffer = PPMBuffer::new(options.width, options.height);
    let camera = Camera::new();

    // Render world
    let render_start_time = std::time::Instant::now();
    for row in (0..ppm_buffer.height).rev() {
        for col in 0..ppm_buffer.width {
            let mut sample_sum = Rgb::<f32>::all(0.0);

            for _s in 0..options.aa_sample_size {
                let sample_col = col as f32 + random_deviation(&mut rng, 0.5);
                let sample_row = row as f32 + random_deviation(&mut rng, 0.5);

                let u = sample_col / ppm_buffer.width as f32;
                let v = sample_row / ppm_buffer.height as f32;
                let look_ray = camera.ray_to_screen(u, v);

                sample_sum = sample_sum + ray_color(&look_ray, MAX_REFLECTS, &mut rng);
            }

            let sampled_color = sample_sum / options.aa_sample_size as f32;
            let color = apply_gamma(sampled_color);
            ppm_buffer.set(row, col, ratio_to_rgb(color));
        }
    }
    profiled_segments.insert("Render", render_start_time.elapsed());

    let file_write_start_time = std::time::Instant::now();
    ppm_buffer.write_to_file(output_filename);
    profiled_segments.insert("File Write", file_write_start_time.elapsed());

    write_meta_to_file(&options, &profiled_segments);
}

#[cfg(test)]
mod raytracer_tests {
    use super::*;

    #[test]
    fn test_ray_hit_sphere() {
        let ray = Ray { origin: Vec3::all(0.0), dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 10.0, y: 0.0, z: -1.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 1.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere1, std::f32::MAX).is_some());
        assert!(ray_hit_sphere(&ray, &sphere2, std::f32::MAX).is_some());
    }

    #[test]
    fn test_ray_miss_sphere() {
        let ray = Ray { origin: Vec3::all(0.0), dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere1 = Sphere { center: Vec3 { x: 11.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        let sphere2 = Sphere { center: Vec3 { x: 0.0, y: 200.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere1, std::f32::MAX).is_none());
        assert!(ray_hit_sphere(&ray, &sphere2, std::f32::MAX).is_none());
    }

    // To combat shadow acne, ray_hits that register too close to the original ray should not
    // qualify as hits
    #[test]
    fn test_ray_miss_sphere_at_origin() {
        let ray = Ray { origin: Vec3::all(0.0), dir: Vec3 { x: 0.0, y: 0.0, z: -1.0 } };
        let sphere = Sphere { center: Vec3 { x: 10.0, y: 0.0, z: 0.0 }, radius: 10.0 };
        assert!(ray_hit_sphere(&ray, &sphere, std::f32::MAX).is_none());
    }
}
