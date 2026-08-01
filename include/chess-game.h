#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "board.h"

typedef enum
{
    NO_PLAYER,
    WHITE_PLAYER,
    BLACK_PLAYER,
} Player;

Player colourToPlayer(const Colour c);

Colour playerToColour(const Player p);

bool equalPlayer(const Player p, const Colour c);

bool areOppositeColour(const Player p, const Colour c);

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

bool* initPositionAt(ChessGame* game, const int x, const int y);

bool* initPositionAtVec(ChessGame* game, const IntVec2D* vec);

void flipChessGame(ChessGame* grid);

void flipCurrChessGame(void);

void setChessGame(const ChessGame* game1, ChessGame* game2);

void resetChessGame(ChessGame* game);

void resetCurrChessGame(void);

Player oppositePlayer(const Player p);

#endif
