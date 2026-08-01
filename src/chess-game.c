#include <string.h>
#include "../include/board.h"
#include "../include/chess-game.h"

Player colourToPlayer(const Colour c)
{
    return (c == WHITE) ? WHITE_PLAYER :
           (c == BLACK) ? BLACK_PLAYER :
           NO_PLAYER;
}

Colour playerToColour(const Player p)
{
    return (p == WHITE_PLAYER) ? WHITE :
           (p == BLACK_PLAYER) ? BLACK :
           NULL_COLOUR;
}

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

bool initPositionAt(const ChessGame* game, const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "There are no pieces here!");

    if (!(y < 2 || y > 5))
        return false;

    return game->inInitPositions[(y > 5) ? (y - 4) : y][x];
}

bool initPositionAtVec(const ChessGame* game, const IntVec2D* vec)
{
    return initPositionAt(game,vec->x,vec->y);
}

bool* initPositionPtrAt(ChessGame* game, const int x, const int y)
{
    static bool notInInitBoundsVar = false;

    assert(isInBoardBounds(x,y) && "There are no pieces here!");

    if (!(y < 2 || y > 5))
        return &notInInitBoundsVar;

    return &game->inInitPositions[(y > 5) ? (y - 4) : y][x];
}

bool* initPositionPtrAtVec(ChessGame* game, const IntVec2D* vec)
{
    return initPositionPtrAt(game,vec->x,vec->y);
}

const ChessGame* flippedChessGame(const ChessGame* game)
{
    static ChessGame gameCopy = {.player = NO_PLAYER};

    setChessGame(game,&gameCopy);

    for (int y=0; y<4; y++)
        for (int x=0; x<8; x++)
        {
            const Piece elmntCp = *getPiecePtrAt(gameCopy.board,x,y);
            *getPiecePtrAt(gameCopy.board,x,y) = *getPiecePtrAt(gameCopy.board,x,7-y);
            *getPiecePtrAt(gameCopy.board,x,7-y) = elmntCp;
        }

    for (int y=0; y<2; y++)
        for (int x=0; x<8; x++)
        {
            const bool elmntCp = *initPositionPtrAt(&gameCopy,x,y);
            *initPositionPtrAt(&gameCopy,x,y) = *initPositionPtrAt(&gameCopy,x,3-y);
            *initPositionPtrAt(&gameCopy,x,3-y) = elmntCp;
        }

    return &gameCopy;
}

void flipChessGame(ChessGame* game)
{
    setChessGame(flippedChessGame(game),game);
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
