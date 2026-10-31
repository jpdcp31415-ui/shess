use crate::vec2::*;

#[derive(Debug, Copy, Clone, PartialEq)]
pub enum PieceColour {
    White,
    Black,
}

impl PieceColour {
    pub fn opposite(&self) -> Self {
        match self {
            PieceColour::White => PieceColour::Black,
            PieceColour::Black => PieceColour::White,
        }
    }
}

#[derive(Debug, Copy, Clone, PartialEq)]
pub enum PieceType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
}

#[derive(Debug)]
enum PieceMove {
    Vector(Vec2<i32>),
    Direction(Vec2<i32>),
}

impl PieceType {
    pub fn moves(&self) -> Vec<PieceMove> {
        match self {
            PieceType::Pawn => vec![
                PieceMove::Vector(Vec2 {x: 0, y: 1}),
                PieceMove::Vector(Vec2 {x: 0, y: 2}),
                PieceMove::Vector(Vec2 {x: -1, y: 1}),
                PieceMove::Vector(Vec2 {x: 1, y: 1}),
            ],

            PieceType::Knight => vec![
                PieceMove::Vector(Vec2 {x:  1, y:  2}),
                PieceMove::Vector(Vec2 {x:  2, y:  1}),
                PieceMove::Vector(Vec2 {x:  2, y: -1}),
                PieceMove::Vector(Vec2 {x:  1, y: -2}),
                PieceMove::Vector(Vec2 {x: -1, y: -2}),
                PieceMove::Vector(Vec2 {x: -2, y: -1}),
                PieceMove::Vector(Vec2 {x: -2, y:  1}),
                PieceMove::Vector(Vec2 {x: -1, y:  2}),
            ],

            PieceType::Bishop => vec![
                PieceMove::Direction(Vec2 {x:  1, y:  1}),
                PieceMove::Direction(Vec2 {x:  1, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y:  1}),
            ],

            PieceType::Rook => vec![
                PieceMove::Direction(Vec2 {x:  0, y:  1}),
                PieceMove::Direction(Vec2 {x:  1, y:  0}),
                PieceMove::Direction(Vec2 {x:  0, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y:  0}),
            ],

            PieceType::Queen => vec![
                PieceMove::Direction(Vec2 {x:  1, y:  1}),
                PieceMove::Direction(Vec2 {x:  1, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y:  1}),
                PieceMove::Direction(Vec2 {x:  0, y:  1}),
                PieceMove::Direction(Vec2 {x:  1, y:  0}),
                PieceMove::Direction(Vec2 {x:  0, y: -1}),
                PieceMove::Direction(Vec2 {x: -1, y:  0}),
            ],

            PieceType::King => vec![
                PieceMove::Vector(Vec2 {x:  1, y:  1}),
                PieceMove::Vector(Vec2 {x:  1, y: -1}),
                PieceMove::Vector(Vec2 {x: -1, y: -1}),
                PieceMove::Vector(Vec2 {x: -1, y:  1}),
                PieceMove::Vector(Vec2 {x:  0, y:  1}),
                PieceMove::Vector(Vec2 {x:  1, y:  0}),
                PieceMove::Vector(Vec2 {x:  0, y: -1}),
                PieceMove::Vector(Vec2 {x: -1, y:  0}),
            ],
        }
    }

    pub fn has_move(&self, vec: &Vec2<i32>) -> bool {
        for i in 0..(self.moves().len()) {
            match self.moves()[i] {
                PieceMove::Direction(mov) =>
                    if vec.is_div_by(&mov) {
                        return true;
                    },
                PieceMove::Vector(mov)    =>
                    if *vec == mov {
                        return true;
                    },
            }
        }
        false
    }

    pub fn is_mult_step(&self) -> bool {
        match self.moves()[0] {
            PieceMove::Direction(..) => true,
            PieceMove::Vector(..)    => false,
        }
    }

    pub fn move_cond<T>(&self) -> impl Fn(T) {
        match self {
            PieceType::Pawn   => |pos|{},
            PieceType::Knight => |pos|{},
            PieceType::Bishop => |pos|{},
            PieceType::Rook   => |pos|{},
            PieceType::Queen  => |pos|{},
            PieceType::King   => |pos|{},
        }
    }

    pub fn loop_all<T>(func: T)
    where
        T: Fn(PieceType)
    {
        func(PieceType::Pawn);
        func(PieceType::Knight);
        func(PieceType::Bishop);
        func(PieceType::Rook);
        func(PieceType::Queen);
        func(PieceType::King);
    }

    pub fn loop_all_moves<T>(&self, func: T)
    where
        T: Fn(&PieceMove)
    {
        for mv in self.moves() {
            func(&mv);
        }
    }

    pub fn all<F>(func: F) -> bool
    where
        F: Fn(Self) -> bool
    {
        func(PieceType::Pawn)   &&
        func(PieceType::Knight) &&
        func(PieceType::Bishop) &&
        func(PieceType::Rook)   &&
        func(PieceType::Queen)  &&
        func(PieceType::King)
    }

    pub fn any<F>(func: F) -> bool
    where
        F: Fn(Self) -> bool
    {
        func(PieceType::Pawn)   ||
        func(PieceType::Knight) ||
        func(PieceType::Bishop) ||
        func(PieceType::Rook)   ||
        func(PieceType::Queen)  ||
        func(PieceType::King)
    }
}

#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Piece {
    pub piece_type: PieceType,
    pub colour: PieceColour,
}

