#include "int-vec-2d.h"
#include "chess-grid.h"

typedef enum
{
    ONE_CHAR,
    TWO_CHARS,
    EMOJI,
} PrintMode;

extern PrintMode gSetPrintMode;

void printPiece(const Piece* p);

extern bool gSetSpaceBetween;

void printBoard(ChessGrid grid);

void printCurrBoard(void);
