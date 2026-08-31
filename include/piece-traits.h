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

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece);

bool isMultStep(const Piece* p);

bool hasMove(const IntVec2D*, const Piece*);

#endif
