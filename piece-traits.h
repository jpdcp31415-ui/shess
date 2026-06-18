#ifndef PIECE_TRAITS_H
#define PIECE_TRAITS_H

#include "piece.h"
#include "int-vec-2d.h"

typedef struct
{
    const Type piece_type;
    const bool mult_steps;
    const IntVec2D* moves;
    const int num_moves;
} PieceTraits;

/*
 for pieces that move around:
        ----
      --    --
     ^        -   <- clock-wise
     S        v
      --    --
        ----
  is the direction that the move member is in
  and the S is the start/first position or 
  at least the closest move to it
*/

const PieceTraits PAWN_TRAITS =
{
    .piece_type = PAWN,
    .mult_steps = false,
    .moves = (const IntVec2D []) {
        {-1,1},{0,1},{1,1},{0,2}
    },
    .num_moves = 4,
};

const PieceTraits KNIGHT_TRAITS = 
{
    .piece_type = KNIGHT,
    .mult_steps = false,
    .moves = (const IntVec2D []) {
        {-2,1},{-1,2},{1,2},{2,1},
        {2,-1},{1,-2},{-1,-2},{-2,-1}
    },
    .num_moves = 8,
};

const PieceTraits BISHOP_TRAITS = 
{
    .piece_type = BISHOP,
    .mult_steps = true,
    .moves = (const IntVec2D []) {
        {-1,1},{1,1},{-1,1},{-1,-1}
    },
    .num_moves = 4,
};

const PieceTraits ROOK_TRAITS =
{
    .piece_type = ROOK,
    .mult_steps = true,
    .moves = (const IntVec2D []) {
        {-1,0},{0,1},{1,0},{-1,0}
    },
    .num_moves = 4,
};

const PieceTraits QUEEN_TRAITS =
{
    .piece_type = QUEEN,
    .mult_steps = true,
    .moves = (const IntVec2D []) {
        {-1,0},{-1,1},{0,1},{1,1},
        {1,0},{1,-1},{0,-1},{-1,-1}
    },
    .num_moves = 8,
};

const PieceTraits KING_TRAITS =
{
    .piece_type = KING,
    .mult_steps = false,
    .moves = (const IntVec2D []) {
        {-1,0},{-1,1},{0,1},{1,1},
        {1,0},{1,-1},{0,-1},{-1,-1}
    },
    .num_moves = 8,
};

const PieceTraits NO_TRAITS =
{
    .piece_type = NULL_TYPE,
    .mult_steps = false,
    .moves = (const IntVec2D []) {{0,0}},
    .num_moves = 1,
};

const PieceTraits* getTraits(const Piece* p)
{
    assertPiece(p);
    
    assert(isBlankSpace(p) && "Error: Blank piece/space shouldn't have traits");

    switch (p->type)
    {
    case PAWN:   return &PAWN_TRAITS;
    case KNIGHT: return &KNIGHT_TRAITS;
    case BISHOP: return &BISHOP_TRAITS;
    case ROOK:   return &ROOK_TRAITS;
    case QUEEN:  return &QUEEN_TRAITS;
    case KING:   return &KING_TRAITS;
    case NULL_TYPE:
    }
    
    // using this to avoid compiler warning/error
    return &NO_TRAITS;
}

#endif
