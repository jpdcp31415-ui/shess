use crate::piece::*;
use crate::board::*;
use crate::vec2::*;

#[derive(Debug, Clone)]
struct Move {
    mover: (Vec2<i32>, BoardSpace),
    attacked: (Vec2<i32>, BoardSpace),
}

impl Move {
    pub fn new() -> Self {
        Self {
            mover: (Vec2::<i32>::new(), BoardSpace::new()),
            attacked: (Vec2::<i32>::new(), BoardSpace::new()),
        }
    }

    pub fn move_vec(&self) -> Vec2<i32> {
        self.mover.0 + self.attacked.0
    }
}

#[derive(Debug, Clone)]
pub struct Game {
    board: Board,
    player: Option<PieceColour>,
    moves: Vec<Move>,
}

impl Game {
    pub const fn new() -> Self {
        Self {
            board: Board::new(),
            player: None,
            moves: Vec::new(),
        }
    }

    pub const fn start_pos() -> Self {
        Self {
            board: Board::start_pos(),
            player: Some(PieceColour::White),
            moves: Vec::new(),
        }
    }
}
