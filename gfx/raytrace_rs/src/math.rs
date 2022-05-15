use std::ops::{Add, Div, Mul, Sub};

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Vec3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

impl Vec3 {
    pub fn all(v: f32) -> Vec3 {
        Vec3 { x: v, y: v, z: v }
    }

    pub fn mag(&self) -> f32 {
        dot_product(*self, *self).sqrt()
    }
}

pub fn dot_product(v1: Vec3, v2: Vec3) -> f32 {
    (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)
}

pub fn unit_vector(v: Vec3) -> Vec3 {
    v / v.mag()
}

pub fn reflect(v: Vec3, n: Vec3) -> Vec3 {
    v - (unit_vector(n) * 2.0 * dot_product(v, n))
}

pub fn lerp<T: Mul<f32, Output = T> + Add<Output = T>>(start: T, end: T, t: f32) -> T {
    assert!(t >= 0.0);
    assert!(t <= 1.0);
    start * (1.0 - t) + end * t
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
