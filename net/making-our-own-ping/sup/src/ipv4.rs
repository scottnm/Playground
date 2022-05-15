#[derive(Debug)]
pub enum ParseAddrError {
    TooFewParts,
    TooManyParts,
    ParsePartError(std::num::ParseIntError),
}

impl From<std::num::ParseIntError> for ParseAddrError {
    fn from(e: std::num::ParseIntError) -> Self {
        Self::ParsePartError(e)
    }
}

impl std::fmt::Display for ParseAddrError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{:?}", self)
    }
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct Addr(pub [u8; 4]);

impl std::fmt::Debug for Addr {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{}.{}.{}.{}", self.0[0], self.0[1], self.0[2], self.0[3])
    }
}

impl std::str::FromStr for Addr {
    type Err = ParseAddrError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut tokens = s.split(".");

        let mut res = Self([0, 0, 0, 0]);
        for part in res.0.iter_mut() {
            *part = tokens.next().ok_or(ParseAddrError::TooFewParts)?.parse()?;
        }

        match tokens.next() {
            Some(_) => Err(ParseAddrError::TooManyParts),
            None => Ok(res),
        }
    }
}
