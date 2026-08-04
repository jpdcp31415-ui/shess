#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "board.h"

#define NULL_PLAYER NULL_COLOUR

typedef Colour Player;

typedef struct
{
    Board board;
    Player player;
    bool inInitPositions[8][8];
} ChessGame;

extern ChessGame gCurrChessGame;

Piece getCurrPieceAt(const int x, const int y);

Piece getCurrPieceAtVec(const IntVec2D* vec);

Piece* getCurrPiecePtrAt(const int x, const int y);

Piece* getCurrPiecePtrAtVec(const IntVec2D* vec);

bool initPositionAt(const ChessGame* game, const int x, const int y);

bool initPositionAtVec(const ChessGame* game, const IntVec2D* vec);

bool* initPositionPtrAt(ChessGame* game, const int x, const int y);

bool* initPositionPtrAtVec(ChessGame* game, const IntVec2D* vec);

void flipChessGame(ChessGame* game);

const ChessGame* flippedKChessGamePtr(const ChessGame* game);

void flipCurrChessGame(void);

void setChessGame(const ChessGame* game1, ChessGame* game2);

void resetChessGame(ChessGame* game);

void resetCurrChessGame(void);

#endif
