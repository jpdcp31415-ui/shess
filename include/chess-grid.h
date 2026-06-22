#include "piece.h"
#include "int-vec-2d.h"

typedef Piece ChessGrid[8][8];

extern const ChessGrid startChessGrid;
extern ChessGrid currBoard;

bool isInBoardBounds(int x, int y);

bool isVecInBoardBounds(const IntVec2D* vec);

Piece* getPieceAt(ChessGrid grid, const int x, const int y);

Piece* getCurrPieceAt(const int x, const int y);

Piece* getPieceAtVec(ChessGrid grid, const IntVec2D* vec);

Piece* getCurrPieceAtVec(const IntVec2D* vec);

typedef enum
{
    ONE_CHAR,
    TWO_CHARS,
    EMOJI,
} PrintMode;

extern PrintMode g_setPrintMode;

void printPiece(const Piece* p);

extern bool g_setSpaceBetween;

void printBoard(ChessGrid grid);

void printCurrBoard(void);

void flipBoard(ChessGrid grid);

void flipCurrBoard(void);

void setBoard(const ChessGrid fromGrid, ChessGrid toGrid);

void resetBoard(ChessGrid grid);

void resetCurrBoard(void);
