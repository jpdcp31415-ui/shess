#ifndef PIECE_H
#define PIECE_H

#define ENABLE_ASSERT 1

#if ENABLE_ASSERT
    #include <assert.h>
#else
    #define assert(x)
#endif

#include <stdbool.h>
#include <ctype.h>

typedef enum
{
    NULL_COLOUR,
    WHITE,
    BLACK,
} Colour;

typedef enum
{
    NULL_TYPE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
} Type;

typedef struct
{
    Colour colour;
    Type type;
} Piece;

bool isBlankSpace(const Piece* p);

void assertPiece(const Piece* p);

const char* getDefPieceEmoji(const Piece* p);

Colour oppositeColour(const Colour c);

bool g_setInvertPieceColours;

const char* getPieceEmoji(const Piece* p);

bool g_setWhiteIsUpper;
int g_setNumChPiece;

char getPiece1Ch(const Piece* p);

const char* getPiece2Ch(const Piece* p);

#endif
