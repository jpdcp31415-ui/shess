#include "../include/piece.h"

bool equalPiece(const Piece* p1, const Piece* p2)
{
    return (p1->colour == p2->colour) &&
           (p1->type   == p2->type);
}

bool isBlankSpace(const Piece* p)
{
    return equalPiece(p,&(Piece){NULL_COLOUR,NULL_TYPE});
}

void assertPiece(const Piece* p)
{
    const bool isValid =  (isBlankSpace(p)   )  ||
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
