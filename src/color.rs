extern crate num;

use std::ops::{Add, Div, Mul};

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Rgb<T> {
    pub r: T,
    pub g: T,
    pub b: T,
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

pub fn rgb_cast<FT: num::NumCast, TT: num::NumCast>(from: Rgb<FT>) -> Rgb<TT> {
    Rgb {
        r: num::cast(from.r).unwrap(),
        g: num::cast(from.g).unwrap(),
        b: num::cast(from.b).unwrap(),
    }
}

pub fn ratio_to_rgb(rgbf: Rgb<f32>) -> Rgb<u8> {
    const ALMOST_MAX: f32 = (std::u8::MAX as f32) - 0.001;
    rgb_cast(rgbf * ALMOST_MAX)
}

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Hsv {
    pub h: u16, // [0, 360)
    pub s: f32,
    pub v: f32,
}

pub fn hsv_to_rgb(color: Hsv) -> Rgb<u8> {
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

#[cfg(test)]
mod color_tests {
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
