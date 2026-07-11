#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "int-vec-2d.h"

typedef Piece Board[8][8];

Piece* getPieceAt(Board board, const int x, const int y);

Piece* getPieceAtVec(Board board, const IntVec2D* vec);

bool isInBoardBounds(const int x, const int y);

bool isVecInBoardBounds(const IntVec2D* vec);

#endif
