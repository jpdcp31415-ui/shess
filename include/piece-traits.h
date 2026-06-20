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

const PieceTraits PAWN_TRAITS;

const PieceTraits KNIGHT_TRAITS;

const PieceTraits BISHOP_TRAITS;

const PieceTraits ROOK_TRAITS;

const PieceTraits QUEEN_TRAITS;

const PieceTraits KING_TRAITS;

extern const PieceTraits NO_TRAITS;

const PieceTraits* getTraits(const Piece* p);

#endif
