#include <string.h>
#include "../include/board.h"
#include "../include/chess-game.h"

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

ChessGame gCurrChessGame =
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

// All of these functions for ChessGame

Piece* getCurrPieceAt(const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(gCurrChessGame.board[y][x]);
}

Piece* getCurrPieceAtVec(const IntVec2D* vec)
{
    return getPieceAtVec(gCurrChessGame.board,vec);
}

bool* initPositionAt(ChessGame* game, const int x, const int y)
{
    assert((x >= 0 && x <= 7) &&
           (y == 0 || y == 1  ||
            y == 6 || y == 7) &&
           "Position chosen is an empty space");
    return &game->inInitPositions[y][x];
}

void flipChessGame(ChessGame* game)
{
    for (int y=0; y<4; y++)
        for (int x=0; x<8; x++)
        {
            const Piece elmntCp = *getPieceAt(game->board,x,y);
            *getPieceAt(game->board,x,y) = *getPieceAt(game->board,x,7-y);
            *getPieceAt(game->board,x,7-y) = elmntCp;
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
