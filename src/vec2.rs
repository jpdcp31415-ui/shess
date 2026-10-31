use std::ops::Add;
use std::ops::Sub;
use std::ops::Mul;
use std::ops::Div;
use std::ops::Neg;

#[derive(Debug, PartialEq, Copy, Clone)]
pub struct Vec2<T> {
    pub x: T,
    pub y: T,
}

impl Vec2<usize> {
    pub fn new() -> Self {
        Self {
            x: 0,
            y: 0,
        }
    }

    pub fn invert_board_x(&self) -> Self {
        Self {
            x: 7-self.x,
            y: self.y,
        }
    }

    pub fn invert_board_y(&self) -> Self {
        Self {
            x: self.x,
            y: 7-self.y,
        }
    }
}

impl Vec2<i32> {
    pub fn new() -> Self {
        Self {
            x: 0,
            y: 0,
        }
    }

    fn is_horiz(&self) -> bool {
        self.x != 0 && self.y == 0
    }

    fn is_vert(&self) -> bool {
        self.x == 0 && self.y != 0
    }

    fn is_diag(&self) -> bool {
        self.x != 0 && self.y != 0
    }

    pub fn is_div_by(&self, other: &Self) -> bool {
        if self.is_horiz() && other.is_horiz() {
            self.x % other.x == 0
        } else if self.is_vert() && other.is_vert() {
            self.y % other.y == 0
        } else if self.is_diag() && other.is_diag() {
            self.x % other.x == 0 && self.y % other.y == 0
        } else {
            false
        }
    }
}

impl<T> Add for Vec2<T>
where
    T: Add<Output = T>
{
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

impl<T> Sub for Vec2<T>
where
    T: Sub<Output = T>
{
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

impl<T> Mul<T> for Vec2<T>
where
    T: Mul<Output = T> + Copy
{
    type Output = Self;
    fn mul(self, n: T) -> Self::Output {
        Self::Output {
            x: self.x * n,
            y: self.y * n,
        }
    }
}

impl<T> Div<T> for Vec2<T>
where
    T: Div<Output = T> + Copy
{
    type Output = Self;
    fn div(self, n: T) -> Self::Output {
        Self::Output {
            x: self.x / n,
            y: self.y / n,
        }
    }
}

impl<T> Neg for Vec2<T>
where
    T: Neg<Output = T>
{
    type Output = Self;
    fn neg(self) -> Self::Output {
        Self::Output {
            x: -self.x,
            y: -self.y,
        }
    }
}
