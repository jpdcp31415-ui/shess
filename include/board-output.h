#ifndef BOARD_OUTPUT_H
#define BOARD_OUTPUT_H

#include "int-vec-2d.h"
#include "board.h"

void printPiece(const Piece* p);

const char* getDefPieceEmoji(const Piece* p);

const char* getPieceEmoji(const Piece* p);

char getPiece1Ch(const Piece* p);

const char* getPiece2Ch(const Piece* p);

void printBoard(const Board board);

/* void runSetup(void); */

#endif
