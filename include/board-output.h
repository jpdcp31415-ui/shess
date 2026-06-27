#ifndef CHESS_GRID_OUTPUT_H
#define CHESS_GRID_OUTPUT_H

#include "int-vec-2d.h"
#include "board.h"

typedef enum
{
    ONE_CHAR,
    TWO_CHARS,
    EMOJI,
} PrintMode;

void printPiece(const Piece* p);

const char* getDefPieceEmoji(const Piece* p);

const char* getPieceEmoji(const Piece* p);

char getPiece1Ch(const Piece* p);

const char* getPiece2Ch(const Piece* p);

void printBoard(Board grid);

void printCurrBoard(void);

void runSetup(void);

#endif
