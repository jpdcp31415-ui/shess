#ifndef PIECE_H
#define PIECE_H

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

bool equalPiece(const Piece* p1, const Piece* p2);

bool isBlankSpace(const Piece* p);

void assertPiece(const Piece* p);

Colour oppositeColour(const Colour c);

#endif
