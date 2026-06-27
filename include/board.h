#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "int-vec-2d.h"

typedef Piece Board[8][8];

extern Board gCurrBoard;

bool isInBoardBounds(int x, int y);

bool isVecInBoardBounds(const IntVec2D* vec);

Piece* getPieceAt(Board grid, const int x, const int y);

Piece* getCurrPieceAt(const int x, const int y);

Piece* getPieceAtVec(Board grid, const IntVec2D* vec);

Piece* getCurrPieceAtVec(const IntVec2D* vec);

void flipBoard(Board grid);

void flipCurrBoard(void);

void setBoard(const Board fromGrid, Board toGrid);

void resetBoard(Board grid);

void resetCurrBoard(void);

#endif
