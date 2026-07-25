#include "../include/piece-traits.h"

static const PieceTraits kTraitsArray[] =
{
    {
        .pieceType = PAWN,
        .multSteps = false,
        .moves = (const IntVec2D []) {
            {-1,-1},{0,-1},{1,-1},{0,-2}
        },
        .numMoves = 4,
    },

    {
        .pieceType = KNIGHT,
        .multSteps = false,
        .moves = (const IntVec2D []) {
            {-2,1},{-1,2},{1,2},{2,1},
            {2,-1},{1,-2},{-1,-2},{-2,-1}
        },
        .numMoves = 8,
    },

    {
        .pieceType = BISHOP,
        .multSteps = true,
        .moves = (const IntVec2D []) {
            {-1,1},{1,1},{1,-1},{-1,-1}
        },
        .numMoves = 4,
    },

    {
        .pieceType = ROOK,
        .multSteps = true,
        .moves = (const IntVec2D []) {
            {-1,0},{0,1},{1,0},{0,-1}
        },
        .numMoves = 4,
    },

    {
        .pieceType = QUEEN,
        .multSteps = true,
        .moves = (const IntVec2D []) {
            {-1,0},{-1,1},{0,1},{1,1},
            {1,0},{1,-1},{0,-1},{-1,-1}
        },
        .numMoves = 8,
    },

    {
        .pieceType = KING,
        .multSteps = false,
        .moves = (const IntVec2D []) {
            {-2,0},{-1,0},{-1,1},{0,1},{0,2},
            {1,1},{1,0},{1,-1},{0,-1},{-1,-1},
        },
        .numMoves = 10,
    }
};

const PieceTraits* getTraits(const Piece* p)
{
    assertPiece(p);
    assert(!isBlankSpace(p) && "Error: Blank piece/space shouldn't have traits");
    return &kTraitsArray[p->type-1];
}

bool hasMove(const IntVec2D* move, const Piece* piece)
{
    assertPiece(piece);

    const PieceTraits* traits = getTraits(piece);
    for (int i = 0; i < traits->numMoves; i++)
    {
        const IntVec2D moveMatch = traits->moves[i];
        
        if (traits->multSteps &&
            isVecDivByVec(move,&moveMatch))
            return true;

        if (equalVecs(move,&moveMatch))
            return true;
    }

    return false;
}
