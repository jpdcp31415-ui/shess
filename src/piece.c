#include "../include/piece.h"
#include "../include/assert-toggle.h"

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

    ASSERT_FMT(isValid, "Piece does not exist in the real world\n"
                        "Piece values as ints: colour %d, type: %d", (int)p->colour, (int)p->type);
}


Colour oppositeColour(const Colour c)
{
    if (c != NULL_COLOUR)
        return ((c == WHITE) ? BLACK : WHITE);

    return NULL_COLOUR;
}
