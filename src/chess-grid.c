#include <string.h>
#include <stdio.h>
#include "../include/chess-grid.h"

typedef Piece ChessGrid[8][8];

const ChessGrid startChessGrid = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN}, // / 1st
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}}, // \ row
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},      // / 2nd
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},     // \ row
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},      // / 8th
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},     // \ row
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN}, // / 7th
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}}, // \ row
};

ChessGrid currBoard = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN}, // / 1st
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}}, // \ row
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},      // / 2nd
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},     // \ row
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},      // / 8th
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},     // \ row
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN}, // / 7th
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}}, // \ row
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

Piece* getPieceAt(ChessGrid grid, const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(grid[y][x]);
}

Piece* getCurrPieceAt(const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(currBoard[y][x]);
}

Piece* getPieceAtVec(ChessGrid grid, const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(grid[vec->y][vec->x]);
}

Piece* getCurrPieceAtVec(const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(currBoard[vec->y][vec->x]);
}


PrintMode g_setPrintMode = EMOJI;

void printPiece(const Piece* p)
{
    switch (g_setPrintMode)
    {
    case ONE_CHAR:  printf("%c", getPiece1Ch(p));   break;
    case TWO_CHARS: printf("%s", getPiece2Ch(p));   break;
    case EMOJI:     printf("%s", getPieceEmoji(p)); break;
    }
}

bool g_setSpaceBetween = true;

void printBoard(ChessGrid grid)
{
    for (int y=0; y<8; y++)
    {
        for (int x=0; x<8; x++)
        {
            printPiece(getPieceAt(grid,x,y));
            if (g_setSpaceBetween)
                printf(" ");
        }
        printf("\n");
    }
}

void printCurrBoard(void)
{
    printBoard(currBoard);
}

void flipBoard(ChessGrid grid)
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
    flipBoard(currBoard);
}

void setBoard(const ChessGrid fromGrid, ChessGrid toGrid)
{
    memcpy(toGrid, fromGrid, sizeof(ChessGrid));
}

void resetBoard(ChessGrid grid)
{
    memcpy(&grid, startChessGrid, sizeof(ChessGrid));
}

void resetCurrBoard(void)
{
    resetBoard(currBoard);
}
