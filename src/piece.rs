use crate::vec2::Vec2;
use crate::game::{Game, InvalidMove, PartUncheckedMove, UncheckedMove, ValidMove};

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

#[derive(PartialEq, Copy, Clone, Debug)]
pub enum PieceMove {
    Vector(Vec2<i32>),
    Direction(Vec2<i32>, Option<u32>),
}

impl PieceMove {
    pub fn total_vec(&self) -> Vec2<i32> {
        match self {
            PieceMove::Vector(mov) => *mov,
            PieceMove::Direction(mov, steps) => *mov * steps.unwrap().try_into().unwrap(),
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub enum PieceSpecificErr {
    PawnCannotMoveDiag,
    PawnIsNotInStartingPosition,
    PawnCannotAttackInFront,
    RookHasMovedBefore,
    PiecesInWayOfMove,
}

impl PieceSpecificErr {
    pub fn reason(&self) -> &'static str {
        match self {
            PieceSpecificErr::PawnCannotMoveDiag          => "Pawn cannot move diagonally because there is not piece to attack there",
            PieceSpecificErr::PawnIsNotInStartingPosition => "Pawn cannot move 2 spaces forward because it has moved before",
            PieceSpecificErr::PawnCannotAttackInFront     => "Pawn cannot attack the pieces that are is in front of it",
            PieceSpecificErr::RookHasMovedBefore          => "Cannot castle because rook has moved before",
            PieceSpecificErr::PiecesInWayOfMove           => "There are pieces is way of your move",
        }
    }
}

impl PieceType {
    pub fn moves(&self) -> Vec<PieceMove> {
        match self {
            PieceType::Pawn => vec![
                PieceMove::Vector(Vec2 {x:  0, y: -1}),
                PieceMove::Vector(Vec2 {x:  0, y: -2}),
                PieceMove::Vector(Vec2 {x: -1, y: -1}),
                PieceMove::Vector(Vec2 {x:  1, y: -1}),
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
                PieceMove::Direction(Vec2 {x:  1, y:  1}, None),
                PieceMove::Direction(Vec2 {x:  1, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y:  1}, None),
            ],

            PieceType::Rook => vec![
                PieceMove::Direction(Vec2 {x:  0, y:  1}, None),
                PieceMove::Direction(Vec2 {x:  1, y:  0}, None),
                PieceMove::Direction(Vec2 {x:  0, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y:  0}, None),
            ],

            PieceType::Queen => vec![
                PieceMove::Direction(Vec2 {x:  1, y:  1}, None),
                PieceMove::Direction(Vec2 {x:  1, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y:  1}, None),
                PieceMove::Direction(Vec2 {x:  0, y:  1}, None),
                PieceMove::Direction(Vec2 {x:  1, y:  0}, None),
                PieceMove::Direction(Vec2 {x:  0, y: -1}, None),
                PieceMove::Direction(Vec2 {x: -1, y:  0}, None),
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

    pub fn has_move(&self, mov: Vec2<i32>) -> bool {
        self.moves().iter().find(|m|
            match m {
                PieceMove::Vector(v)       => mov == *v,
                PieceMove::Direction(d, s) => mov == *d * s.unwrap() as i32,
            }
        ).is_some()
    }

    pub fn is_mult_step(&self) -> bool {
        match self.moves()[0] {
            PieceMove::Direction(..) => true,
            PieceMove::Vector(..)    => false,
        }
    }

    pub fn move_cond(&self) -> Box<dyn Fn(&Game, &PartUncheckedMove) -> Result<(), PieceSpecificErr>> {
        match self {
            PieceType::Pawn   =>
                Box::new(|game: &Game, mov: &PartUncheckedMove| {
                    match mov.move_vec() {
                        Vec2 {x: 0, y: -1} =>
                            if mov.attacked().is_blank() {
                                Ok(())
                            } else {
                                Err(PieceSpecificErr::PawnCannotAttackInFront)
                            },
                        Vec2 {x: -1, y: -1} | Vec2 {x: 1, y: -1} =>
                            if !mov.attacked().is_blank() {
                                Ok(())
                            } else {
                                Err(PieceSpecificErr::PawnCannotMoveDiag)
                            },
                        Vec2 {x: 0, y: -2} =>
                            /* Add error for not the pawn not being in the starting position */
                            if !game.board().at(mov.from_pos() + Vec2 {x: 0, y: -1}).is_blank() {
                                Err(PieceSpecificErr::PiecesInWayOfMove)
                            } else if game.board().at(mov.to_pos()).is_blank() {
                                Err(PieceSpecificErr::PawnCannotAttackInFront)
                            } else {
                                Ok(())
                            }
                        _ => Ok(()),
                    }
                }),

            PieceType::Knight =>
                Box::new(|_: &Game, _: &PartUncheckedMove| {
                    Ok(())
                }),

            PieceType::Bishop | PieceType::Rook | PieceType::Queen =>
                Box::new(|game: &Game, mov: &PartUncheckedMove| {
                    if mov.is_clear(game) {
                        return Ok(());
                    } else {
                        return Err(PieceSpecificErr::PiecesInWayOfMove);
                    }
                }),

            PieceType::King   =>
                Box::new(|game: &Game, mov: &PartUncheckedMove| {
                    todo!()
                }),
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
        T: Fn(Vec2<i32>)
    {
        for mv in self.moves() {
            match mv {
                PieceMove::Vector(v)          => func(v),
                PieceMove::Direction(d, None) =>
                    for _ in 1..8 {
                        func(d);
                    }
                _ => panic!("Please do not provide a steps number for the piece move array"),
            }
        }

    }

    pub fn any_moves<T>(&self, func: T) -> bool
    where
        T: Fn(Vec2<i32>) -> bool
    {
        for mv in self.moves() {
            match mv {
                PieceMove::Vector(v) if func(v) => { return true; },
                PieceMove::Direction(d, None) =>
                    for i in 1..8 {
                        if func(d * i) {
                            return true;
                        }
                    }
                PieceMove::Direction(_, Some(_)) => panic!("Please do not provide a steps number for the piece move array"),
                _ => (),
            }
        }
        false
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
