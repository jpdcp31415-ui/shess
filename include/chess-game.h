#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "board.h"

typedef enum
{
    WHITE_PLAYER,
    BLACK_PLAYER,
} Player;

typedef struct
{
    Board board;
    Player player;
    bool inInitPositions[8][8];
} ChessGame;

extern ChessGame gCurrChessGame;

Piece* getCurrPieceAt(const int x, const int y);

Piece* getCurrPieceAtVec(const IntVec2D* vec);

void flipBoard(Board grid);

void flipCurrBoard(void);

void setChessGame(const Board fromGrid, Board toGrid);

void resetChessGame(Board grid);

void resetCurrChessGame(void);

#endif
