use crate::piece::{PieceColour, PieceType, Piece};
use crate::vec2::Vec2;

#[derive(Debug, PartialEq, Copy, Clone)]
pub enum BoardSpace {
    Blank,
    Some(Piece),
}

impl BoardSpace {
    pub fn new() -> Self {
        Self::Blank
    }

    pub fn is_blank(&self) -> bool {
        *self == BoardSpace::Blank
    }

    pub fn as_piece(&self) -> Piece {
        match self {
            BoardSpace::Some(p) => *p,
            _ => panic!("Cannot get piece from blank space"),
        }
    }
}

#[derive(Debug, Clone)]
pub struct Board {
    data: [[BoardSpace; 8]; 8],
}

impl Board {
    pub fn new() -> Self {
        type BS = BoardSpace;
        Board {
            data: [
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
            ]
        }
    }

    pub fn start_pos() -> Self {
        type BS = BoardSpace;
        type PT = PieceType;
        type PC = PieceColour;
        Board {
            data: [
                [
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Rook,   }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Knight, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Bishop, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::King,   }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Queen,  }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Bishop, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Knight, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Rook,   })
                ],

                [
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::Black, piece_type: PT::Pawn, })
                ],

                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],
                [
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank,
                    BS::Blank, BS::Blank, BS::Blank, BS::Blank
                ],

                [
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Pawn, })
                ],

                [
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Rook,   }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Knight, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Bishop, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::King,   }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Queen,  }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Bishop, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Knight, }),
                    BS::Some( Piece { colour: PC::White, piece_type: PT::Rook,   })
                ],
            ]
        }
    }            

    pub fn at(&self, pos: Vec2<i32>) -> &BoardSpace {
        if pos.is_in_board_bounds() {
            return &self.data[pos.y as usize][pos.x as usize];
        }

        panic!("Cannot get BoardSpace out of bounds at: {{{}, {}}}", pos.x, pos.y);
    }

    pub fn at_mut(&mut self, pos: Vec2<i32>) -> &mut BoardSpace {
        if pos.is_in_board_bounds() {
            return &mut self.data[pos.y as usize][pos.x as usize];
        }

        panic!("Cannot get BoardSpace out of bounds at: {{{}, {}}}", pos.x, pos.y);
    }

    pub fn flip_ver(&mut self) {
        for y in 0..4 {
            for x in 0..8 {
                let pos = Vec2{x,y};
                *self.at_mut(pos) = *self.at(pos.invert_board_y());
            }
        }
    }

    pub fn king_pos(&self, col: PieceColour) -> Vec2::<i32> {
        for y in 0..8 {
            for x in 0..8 {
                let loop_space = self.at(Vec2{x,y});
                match loop_space {
                    BoardSpace::Some(Piece {piece_type: PieceType::King, ..})
                        if col == loop_space.as_piece().colour => return Vec2 {x, y},
                    _ => continue,
                }
            }
        }

        panic!("Could not find the king");
    }
}
