#include <string.h>
#include "../include/board.h"

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

Piece* getPieceAt(Board board, const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(board[y][x]);
}

Piece* getPieceAtVec(Board board, const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(board[vec->y][vec->x]);
}
