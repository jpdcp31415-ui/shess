#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "board.h"

typedef enum
{
    NOT_PLAYING,
    WHITE_PLAYER,
    BLACK_PLAYER,
} Player;

typedef struct
{
    Board board;
    Player player;
    bool inInitPositions[8][8];
} ChessGame;

extern Board gCurrChessGame;

/* bool isInBoardBounds(int x, int y); */
/*  */
/* bool isVecInBoardBounds(const IntVec2D* vec); */
/*  */
/* Piece* getPieceAt(Board grid, const int x, const int y); */
/*  */
/* Piece* getCurrPieceAt(const int x, const int y); */
/*  */
/* Piece* getPieceAtVec(Board grid, const IntVec2D* vec); */
/*  */
/* Piece* getCurrPieceAtVec(const IntVec2D* vec); */
/*  */
/* void flipBoard(Board grid); */
/*  */
/* void flipCurrBoard(void); */
/*  */
/* void setBoard(const Board fromGrid, Board toGrid); */
/*  */
/* void resetBoard(Board grid); */
/*  */
/* void resetCurrBoard(void); */
/*  */
#endif
