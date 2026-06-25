#ifndef PIECE_TRAITS_H
#define PIECE_TRAITS_H

#include "piece.h"
#include "int-vec-2d.h"

typedef struct
{
    const Type pieceType;
    const bool multSteps;
    const IntVec2D* moves;
    const int numMoves;
} PieceTraits;

const PieceTraits* getTraits(const Piece*);

bool hasMove(const IntVec2D*, const Piece*);

#endif
