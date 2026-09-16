#ifndef PIECE_TRAITS_H
#define PIECE_TRAITS_H

#include "piece.h"
#include "int-vec-2d.h"

typedef struct
{
    const Type pieceType;
    const bool multSteps;
    const IntVec2D* moves;
    const IntVec2D* directions;
    const int numMoves;
    const int numDirecs;
} PieceTraits;

const IntVec2D* getMoves(const Piece* p);

int getNumMoves(const Piece* p);

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece);

bool isMultStep(const Piece* p);

bool hasMove(const IntVec2D*, const Piece*);

#endif
