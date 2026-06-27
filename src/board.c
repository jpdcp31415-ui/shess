#include <string.h>
#include "../include/board.h"

typedef Piece Board[8][8];

static const Board kStartBoard = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN},
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}},
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},     
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},    
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},     
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},    
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN},
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}},
};

Board gCurrBoard = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN},
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}},
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},     
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},    
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},     
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},    
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN},
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}},
};

bool isInBoardBounds(int x, int y)
{
    return ((x >= 0 && x < 8) &&
            (y >= 0 && y < 8));
}

bool isVecInBoardBounds(const IntVec2D* vec)
{
    return ((vec->x >= 0 && vec->x < 8) &&
            (vec->y >= 0 && vec->y < 8));
}

Piece* getPieceAt(Board grid, const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(grid[y][x]);
}

Piece* getCurrPieceAt(const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(gCurrBoard[y][x]);
}

Piece* getPieceAtVec(Board grid, const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(grid[vec->y][vec->x]);
}

Piece* getCurrPieceAtVec(const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(gCurrBoard[vec->y][vec->x]);
}

void flipBoard(Board grid)
{
    for (int y=0; y<4; y++)
        for (int x=0; x<8; x++)
        {
            const Piece elmntCp = *getPieceAt(grid,x,y);
            *getPieceAt(grid,x,y) = *getPieceAt(grid,x,7-y);
            *getPieceAt(grid,x,7-y) = elmntCp;
        }
}

void flipCurrBoard(void)
{
    flipBoard(gCurrBoard);
}

void setBoard(const Board fromGrid, Board toGrid)
{
    memcpy(toGrid, fromGrid, sizeof(Board));
}

void resetBoard(Board grid)
{
    memcpy(&grid, kStartBoard, sizeof(Board));
}

void resetCurrBoard(void)
{
    resetBoard(gCurrBoard);
}
