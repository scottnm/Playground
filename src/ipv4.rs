#[repr(C)]
#[derive(Clone, Copy)]
pub struct Addr(pub [u8; 4]);

impl std::fmt::Debug for Addr {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{}.{}.{}.{}", self.0[0], self.0[1], self.0[2], self.0[3])
    }
}
