#include <string.h>
#include "../include/board.h"
#include "../include/chess-game.h"

bool equalPlayer(const Player p, const Colour c)
{
    return (p == WHITE_PLAYER && c == WHITE) ||
           (p == BLACK_PLAYER && c == BLACK);
}

bool areOppositeColour(const Player p, const Colour c)
{
    return (p == WHITE_PLAYER && c == BLACK) ||
           (p == BLACK_PLAYER && c == WHITE);
}

static const ChessGame kStartChessGame =
{
    .board = {
        {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN},
         {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}},
        {{BLACK,PAWN},{BLACK,PAWN  },{BLACK,PAWN  },{BLACK,PAWN},     
         {BLACK,PAWN},{BLACK,PAWN  },{BLACK,PAWN  },{BLACK,PAWN}},    
    [6]={{WHITE,PAWN},{WHITE,PAWN  },{WHITE,PAWN  },{WHITE,PAWN},     
         {WHITE,PAWN},{WHITE,PAWN  },{WHITE,PAWN  },{WHITE,PAWN}},    
        {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN},
         {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}},
    },

    .player = WHITE_PLAYER,

    .inInitPositions = 
    {
        {true,true,true,true,true,true,true,true}, // black player side
        {true,true,true,true,true,true,true,true},
        {true,true,true,true,true,true,true,true},
        {true,true,true,true,true,true,true,true}, // white player side
    },
};

ChessGame gCurrChessGame = {.player = WHITE_PLAYER}; // game is set in the commandLoop function

Piece getCurrPieceAt(const int x, const int y)
{
    return getPieceAt(gCurrChessGame.board,x,y);
}

Piece getCurrPieceAtVec(const IntVec2D* vec)
{
    return getPieceAtVec(gCurrChessGame.board,vec);
}
Piece* getCurrPtrPieceAt(const int x, const int y)
{
    return getPiecePtrAt(gCurrChessGame.board,x,y);
}

Piece* getCurrPiecePtrAtVec(const IntVec2D* vec)
{
    return getPiecePtrAtVec(gCurrChessGame.board,vec);
}

bool* initPositionAt(ChessGame* game, const int x, const int y)
{
    static bool notInInitBoundsVar = false;

    const bool inInitBounds =
        (x >= 0 && x <= 7) &&
        (y == 0 || y == 1  ||
         y == 6 || y == 7);

    return (inInitBounds ? &game->inInitPositions[y][x] : &notInInitBoundsVar);
}

bool* initPositionAtVec(ChessGame* game, const IntVec2D* vec)
{
    return initPositionAt(game,vec->x,vec->y);
}

void flipChessGame(ChessGame* game)
{
    for (int y=0; y<4; y++)
        for (int x=0; x<8; x++)
        {
            const Piece elmntCp = *getPiecePtrAt(game->board,x,y);
            *getPiecePtrAt(game->board,x,y) = *getPiecePtrAt(game->board,x,7-y);
            *getPiecePtrAt(game->board,x,7-y) = elmntCp;
        }

    for (int y=0; y<2; y++)
        for (int x=0; x<8; x++)
        {
            const bool elmntCp = *initPositionAt(game,x,y);
            *initPositionAt(game,x,y) = *initPositionAt(game,x,7-y);
            *initPositionAt(game,x,7-y) = elmntCp;
        }
}

void flipCurrChessGame(void)
{
    flipChessGame(&gCurrChessGame);
}

void setChessGame(const ChessGame* game1, ChessGame* game2)
{
    memcpy(game2, game1, sizeof(ChessGame));
}

void resetChessGame(ChessGame* game)
{
    setChessGame(&kStartChessGame, game);
}

void resetCurrChessGame(void)
{
    resetChessGame(&gCurrChessGame);
}

Player oppositePlayer(const Player p)
{
    assert((p == WHITE_PLAYER || p == BLACK_PLAYER) && "Player is invalid");
    return (p == WHITE_PLAYER) ? BLACK_PLAYER : WHITE_PLAYER;
}
