#include "piece.h"
#include "int-vec-2d.h"

typedef Piece ChessGrid[8][8];

extern ChessGrid gCurrBoard;

bool isInBoardBounds(int x, int y);

bool isVecInBoardBounds(const IntVec2D* vec);

Piece* getPieceAt(ChessGrid grid, const int x, const int y);

Piece* getCurrPieceAt(const int x, const int y);

Piece* getPieceAtVec(ChessGrid grid, const IntVec2D* vec);

Piece* getCurrPieceAtVec(const IntVec2D* vec);

void flipBoard(ChessGrid grid);

void flipCurrBoard(void);

void setBoard(const ChessGrid fromGrid, ChessGrid toGrid);

void resetBoard(ChessGrid grid);

void resetCurrBoard(void);
