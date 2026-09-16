#include "../include/piece-traits.h"
#include "../include/assert-toggle.h"

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
            {-1,1},{1,1},{1,-1},{-1,-1},
            {-2,2},{2,2},{2,-2},{-2,-2},
            {-3,3},{3,3},{3,-3},{-3,-3},
            {-4,4},{4,4},{4,-4},{-4,-4},
            {-5,5},{5,5},{5,-5},{-5,-5},
            {-6,6},{6,6},{6,-6},{-6,-6},
            {-7,7},{7,7},{7,-7},{-7,-7},
        },
        .directions = (const IntVec2D []) {
            {-1,1},{1,1},{1,-1},{-1,-1},
        },
        .numMoves = 28,
        .numDirecs = 4,
    },

    {
        .pieceType = ROOK,
        .multSteps = true,
        .moves = (const IntVec2D []) {
            {-1,0},{0,1},{1,0},{0,-1},
            {-2,0},{0,2},{2,0},{0,-2},
            {-3,0},{0,3},{3,0},{0,-3},
            {-4,0},{0,4},{4,0},{0,-4},
            {-5,0},{0,5},{5,0},{0,-5},
            {-6,0},{0,6},{6,0},{0,-6},
            {-7,0},{0,7},{7,0},{0,-7},
        },
        .directions = (const IntVec2D []) {
            {-1,0},{0,1},{1,0},{0,-1},
        },
        .numMoves = 28,
        .numDirecs = 4,
    },

    {
        .pieceType = QUEEN,
        .multSteps = true,
        .moves = (const IntVec2D []) {
            {-1,0},{0,1},{1,0},{0,-1},
            {-2,0},{0,2},{2,0},{0,-2},
            {-3,0},{0,3},{3,0},{0,-3},
            {-4,0},{0,4},{4,0},{0,-4},
            {-5,0},{0,5},{5,0},{0,-5},
            {-6,0},{0,6},{6,0},{0,-6},
            {-7,0},{0,7},{7,0},{0,-7},
            {-1,1},{1,1},{1,-1},{-1,-1},
            {-2,2},{2,2},{2,-2},{-2,-2},
            {-3,3},{3,3},{3,-3},{-3,-3},
            {-4,4},{4,4},{4,-4},{-4,-4},
            {-5,5},{5,5},{5,-5},{-5,-5},
            {-6,6},{6,6},{6,-6},{-6,-6},
            {-7,7},{7,7},{7,-7},{-7,-7},
        },
        .directions = (const IntVec2D []) {
            {-1,0},{-1,1},{0,1},{1,1},
            {1,0},{1,-1},{0,-1},{-1,-1}
        },
        .numMoves = 56,
        .numDirecs = 8,
    },

    {
        .pieceType = KING,
        .multSteps = false,
        .moves = (const IntVec2D []) {
            {-1,0},{-1,1},{0,1},{1,1}, // moves with a 2 are the special "roque" moves
            {1,0},{1,-1},{0,-1},{-1,-1},
            {-2,0},{2,0},
        },
        .numMoves = 10,
    }
};

const PieceTraits* getTraits(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank piece/space shouldn't have traits");
    return &kTraitsArray[p->type-1];
}

const IntVec2D* getMoves(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank piece/space shouldn't have moves");
    return getTraits(p)->moves;
}

int getNumMoves(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank piece/space shouldn't have moves");
    return getTraits(p)->numMoves;
}

int getNumDirecs(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p) && isMultStep(p), "Blank piece/space shouldn't have moves");
    return getTraits(p)->numDirecs;
}

const IntVec2D* getDirections(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p) && isMultStep(p), "Blank piece/space shouldn't have directions");
    return getTraits(p)->moves;
}

IntVec2D getDirecVec(const IntVec2D* move, const Piece* piece)
{
    ASSERT(isMultStep(piece), "Piece is not does not move multiple steps");

    const IntVec2D* directions = getDirections(piece);

    for (int i = 0; i < getNumMoves(piece); i++)
        if (isVecDivByVec(move,&directions[i]))
            return directions[i];

    EXIT_MSG(!"Direction vector not found!");
}

bool isMultStep(const Piece* p)
{
    assertPiece(p);
    ASSERT(!isBlankSpace(p), "Blank piece/space shouldn't have traits");
    return getTraits(p)->multSteps;
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
