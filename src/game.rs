use crate::piece::{PieceColour, PieceType, Piece, PieceMove, PieceSpecificErr};
use crate::board::{Board, BoardSpace};
use crate::vec2::Vec2;

#[derive(Debug, Copy, Clone)]
pub enum MoveErr {
    FromPositionIsNotInBounds,
    MoverPieceIsBlankSpace,
    OppositeMovingPieceColour,
    PieceDoesNotHaveMove,
    ToPositionIsNotInBounds,

    PieceSpecificErr(PieceSpecificErr),

    MoveCausesCheck,
    CannotAttackYourOwnPieces,
    StillInCheckAfterMove,
}

impl MoveErr {
    pub fn reason(&self) -> &'static str {
        match self {
            MoveErr::FromPositionIsNotInBounds   => "Position moving from is out of bound of board",
            MoveErr::MoverPieceIsBlankSpace      => "The piece moving is a blank space in the board",
            MoveErr::OppositeMovingPieceColour   => "Cannot move using the opponent's piece",
            MoveErr::PieceDoesNotHaveMove        => "This piece does not have that move",
            MoveErr::ToPositionIsNotInBounds     => "The position that you want to move to is not in bounds",

            // Piece Specific
            MoveErr::PieceSpecificErr(err) => err.reason(),

            MoveErr::MoveCausesCheck             => "This move causes your king to be in check",
            MoveErr::CannotAttackYourOwnPieces   => "You cannot attack your own pieces",
            MoveErr::StillInCheckAfterMove       => "This move will not get you out of check",
        }
    }
}

#[derive(Debug, Clone)]
pub struct PartUncheckedMove {
    from_pos: Vec2<i32>,
    mover: Piece,
    to_pos: Vec2<i32>,
    attacked: BoardSpace,
}

impl PartUncheckedMove {
    pub fn mover(&self) -> Piece {
        self.mover
    }

    pub fn attacked(&self) -> BoardSpace {
        self.attacked
    }

    pub fn from_pos(&self) -> Vec2<i32> {
        self.from_pos
    }

    pub fn to_pos(&self) -> Vec2<i32> {
        self.to_pos
    }

    pub fn move_vec(&self) -> Vec2<i32> {
        Vec2 {
            x: (self.to_pos().x - self.from_pos().x) as i32,
            y: (self.to_pos().y - self.from_pos().y) as i32,
        }
    }
    fn as_piece_move(&self) -> PieceMove {
        *self.mover.piece_type.moves().iter().find(|m|
            match m {
                PieceMove::Vector(v) =>
                    self.to_pos - self.from_pos == *v,
                PieceMove::Direction(d, s) =>
                    self.to_pos - self.from_pos == *d * s.expect("Do not give a value to the steps here") as i32,
            }
        ).expect("Could not make the move into a PieceMove")
    }

    pub fn is_clear(&self, game: &Game) -> bool {
        match self.as_piece_move() {
            PieceMove::Vector(..) => panic!("Cannot say if move is clear for a non multiple step piece"),
            PieceMove::Direction(direc, steps) => {
                for i in 1..steps.unwrap() {
                    let loop_move = direc * i as i32;
                    let mover_pos = self.from_pos();
                    let loop_piece = game.board().at(mover_pos + loop_move);

                    if !loop_piece.is_blank() {
                        return false;
                    }
                }

                true
            }
        }
    }
}

#[derive(Debug, Clone)]
pub struct UncheckedMove {
    from_pos: Vec2<i32>,
    to_pos: Vec2<i32>,
}

impl UncheckedMove {
    pub fn from_pos(&self) -> Vec2<i32> {
        self.from_pos
    }

    pub fn to_pos(&self) -> Vec2<i32> {
        self.to_pos
    }

    pub fn move_vec(&self) -> Vec2<i32> {
        Vec2 {
            x: (self.to_pos().x - self.from_pos().x) as i32,
            y: (self.to_pos().y - self.from_pos().y) as i32,
        }
    }

    pub fn try_to_valid(game: &Game, unc_mov: Self) ->
        Result<ValidMove, InvalidMove>
    {
        let move_err = unc_mov.is_valid(game);
        if move_err.is_err() {
            return Err(
                InvalidMove {
                    from_pos: unc_mov.from_pos(),
                    to_pos: unc_mov.to_pos(),
                    move_err: move_err.err().unwrap(),
                }
            );
        }

        Ok(ValidMove {
            from_pos: unc_mov.to_pos(),
            mover: game.board().at(unc_mov.from_pos()).as_piece(),
            to_pos: unc_mov.to_pos(),
            attacked: *game.board().at(unc_mov.to_pos()),
        })
    }

    fn to_part_unchecked(&self, board: &Board) -> PartUncheckedMove {
        PartUncheckedMove {
            from_pos: self.from_pos(),
            mover:    board.at(self.to_pos()).as_piece(),
            to_pos:   self.to_pos(),
            attacked: *board.at(self.to_pos()),
        }
    }

    fn is_valid(&self, game: &Game) -> Result<(), MoveErr> {
        if !self.from_pos.is_in_board_bounds() {
            return Err(MoveErr::FromPositionIsNotInBounds);
        }

        let mover_space = game.board().at(self.from_pos());
        if mover_space.is_blank() {
            return Err(MoveErr::MoverPieceIsBlankSpace);
        }

        let mover = mover_space.as_piece();
        if game.player != Some(mover.colour) {
            return Err(MoveErr::OppositeMovingPieceColour);
        }

        let move_vec = self.to_pos() - self.from_pos();
        if mover.piece_type.has_move(move_vec) {
            return Err(MoveErr::PieceDoesNotHaveMove);
        }

        if !self.to_pos().is_in_board_bounds() {
            return Err(MoveErr::ToPositionIsNotInBounds);
        }

        let part_unc_mov = self.to_part_unchecked(game.board());

        if mover.piece_type.move_cond()(game, &part_unc_mov).is_err() {
            return Err(
                MoveErr::PieceSpecificErr(
                    mover.piece_type.move_cond()(game, &part_unc_mov).err().unwrap()
                )
            );
        };

        // Add MoveCausesCheck condition here

        let attacked = game.board().at(self.to_pos());
        match attacked {
            BoardSpace::Blank => (),
            BoardSpace::Some(piece)
                if piece.colour == game.player.unwrap() =>
                    return Err(MoveErr::CannotAttackYourOwnPieces),
            BoardSpace::Some(_) => (),
        }

        Ok(())

        // Add StillInCheckAfterMove condition here
    }

    fn to_invalid(&self, move_err: MoveErr) -> InvalidMove {
        InvalidMove {
            from_pos: self.from_pos(),
            to_pos: self.to_pos(),
            move_err,
        }
    }
}

#[derive(Debug, Clone)]
pub struct InvalidMove {
    from_pos: Vec2<i32>,
    to_pos: Vec2<i32>,
    move_err: MoveErr,
}

#[derive(Debug, Clone)]
pub struct ValidMove {
    from_pos: Vec2<i32>,
    mover: Piece,
    to_pos: Vec2<i32>,
    attacked: BoardSpace,
}

impl ValidMove {
    pub fn mover(&self) -> Piece {
        self.mover
    }

    pub fn attacked(&self) -> BoardSpace {
        self.attacked
    }

    pub fn from_pos(&self) -> Vec2<i32> {
        self.from_pos
    }

    pub fn to_pos(&self) -> Vec2<i32> {
        self.to_pos
    }

    pub fn move_vec(&self) -> Vec2<i32> {
        Vec2 {
            x: (self.to_pos().x - self.from_pos().x) as i32,
            y: (self.to_pos().y - self.from_pos().y) as i32,
        }
    }
}

#[derive(Debug, Clone)]
pub struct Game {
    board: Board,
    player: Option<PieceColour>,
    moves: Vec<ValidMove>,
}

impl Game {
    pub fn new() -> Self {
        Self {
            board: Board::new(),
            player: None,
            moves: Vec::new(),
        }
    }

    pub fn start_pos() -> Self {
        Self {
            board: Board::start_pos(),
            player: Some(PieceColour::White),
            moves: Vec::new(),
        }
    }

    pub fn board(&self) -> &Board {
        &self.board
    }

    pub fn is_in_check(&self) -> bool {
        let king_pos = self.board().king_pos(self.player.unwrap());
        PieceType::any(|p|
            p.any_moves(|m| {
                let poss_move =
                    UncheckedMove::try_to_valid(
                        self,
                        UncheckedMove {
                            from_pos: king_pos + m,
                            to_pos: king_pos,
                        }
                    );
                if poss_move.is_ok() { return true; }
                false
            })
        )
    }
}
