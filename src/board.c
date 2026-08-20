#include <string.h>
#include "../include/board.h"
#include "../include/assert-toggle.h"

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

Piece getPieceAt(const Board board, const int x, const int y)
{
    ASSERT_FMT(isInBoardBounds(x,y),
            "Getting piece that is out of bounds!\n Values: %d, %d", x, y);
    return board[y][x];
}

Piece getPieceAtVec(const Board board, const IntVec2D* vec)
{
    ASSERT_FMT(isInBoardBounds(vec->x,vec->y),
            "Getting piece that is out of bounds!\n Values: %d, %d", vec->x, vec->y);
    return getPieceAt(board,vec->x,vec->y);
}

Piece* getPiecePtrAt(Board board, const int x, const int y)
{
    ASSERT_FMT(isInBoardBounds(x,y), "Getting piece that is out of bounds!\n Values: %d, %d", x, y);
    return &(board[y][x]);
}

Piece* getPiecePtrAtVec(Board board, const IntVec2D* vec)
{
    ASSERT_FMT(isInBoardBounds(vec->x,vec->y),
            "Getting piece that is out of bounds!\n Values: %d, %d", vec->x, vec->y);
    return getPiecePtrAt(board,vec->x,vec->y);
}

const Piece* getKPiecePtrAt(const Board board, const int x, const int y)
{
    ASSERT_FMT(isInBoardBounds(x,y), "There are no pieces here!\n Values: %d, %d", x, y);
    return &(board[y][x]);
}

const Piece* getKPiecePtrAtVec(const Board board, const IntVec2D* vec)
{
    ASSERT_FMT(isInBoardBounds(vec->x,vec->y),
            "Getting piece that is out of bounds!\n Values: %d, %d", vec->x, vec->y);
    return getKPiecePtrAt(board,vec->x,vec->y);
}
