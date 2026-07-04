#include "../include/piece.h"

bool isBlankSpace(const Piece* p)
{
    return (p->colour == NULL_COLOUR &&
            p->type   == NULL_TYPE);
}

void assertPiece(const Piece* p)
{
    const bool isValid =  ( isBlankSpace(p)  )  ||
                         ((p->colour == WHITE   ||
                           p->colour == BLACK)  &&
                          (p->type   >= PAWN    &&
                           p->type   <= KING));

    assert(isValid && "Piece does not exist in the real world");
}


Colour oppositeColour(const Colour c)
{
    if (c != NULL_COLOUR)
        return ((c == WHITE) ? BLACK : WHITE);

    return NULL_COLOUR;
}
