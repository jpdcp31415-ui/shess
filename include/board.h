#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "int-vec-2d.h"

typedef Piece Board[8][8];

bool isInBoardBounds(const int x, const int y);

bool isVecInBoardBounds(const IntVec2D* vec);

Piece getPieceAt(const Board board, const int x, const int y);

Piece getPieceAtVec(const Board board, const IntVec2D* vec);

Piece* getPiecePtrAt(Board board, const int x, const int y);

Piece* getPiecePtrAtVec(Board board, const IntVec2D* vec);

const Piece* getKPiecePtrAt(const Board board, const int x, const int y);

const Piece* getKPiecePtrAtVec(const Board board, const IntVec2D* vec);

IntVec2D whereKingIs(const Board board, const Colour kingColour);

#endif
