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

bool isBlankSpace(const Piece* p)
{
    return (p->colour == NULL_COLOUR &&
            p->type   == NULL_TYPE);
}

void assertPiece(const Piece* p)
{
    const bool isValid = (p->colour == NULL_COLOUR &&
                          p->type   == NULL_TYPE)  ||
                         (p->colour != NULL_COLOUR &&
                          p->type   != NULL_TYPE);

    assert(isValid && "Piece does not exist in the real world");
}

// unicode character range: \u2654-F
// order: king, queen, rook, bishop, knight, pawn
const char* getDefPieceEmoji(const Piece* p)
{
    assertPiece(p);

    if (isBlankSpace(p)) return " ";
    
    if (p->colour == WHITE)
        switch (p->type)
        {
        case PAWN:   return "\u2659";
        case KNIGHT: return "\u2658";
        case BISHOP: return "\u2657";
        case ROOK:   return "\u2656";
        case QUEEN:  return "\u2655";
        case KING:   return "\u2654";
        case NULL_TYPE:;
        }
    else if (p->colour == BLACK)
        switch (p->type)
        {
        case PAWN:   return "\u265F";
        case KNIGHT: return "\u265E";
        case BISHOP: return "\u265D";
        case ROOK:   return "\u265C";
        case QUEEN:  return "\u265B";
        case KING:   return "\u265A";
        case NULL_TYPE:;
       }
    
    assert(false && "Error: piece was not asserted!\n");

    return "HOW DID YOU GET HERE!";
}

Colour oppositeColour(const Colour c)
{
    if (c != NULL_COLOUR)
        return ((c == WHITE) ? BLACK : WHITE);

    return NULL_COLOUR;
}

bool g_setInvertPieceColours = false;

const char* getPieceEmoji(const Piece* p)
{
    if (g_setInvertPieceColours)
        return getDefPieceEmoji(&(const Piece){
                oppositeColour(p->colour),
                p->type});

    return getDefPieceEmoji(p);
}

bool g_setWhiteIsUpper = false;
int g_setNumChPiece = 1;

char getPiece1Ch(const Piece* p)
{
    assertPiece(p);

    char ch = '\0';

    switch (p->type)
    {
    case PAWN:      ch = 'p'; break;
    case KNIGHT:    ch = 'h'; break;
    case BISHOP:    ch = 'b'; break;
    case ROOK:      ch = 'r'; break;
    case QUEEN:     ch = 'q'; break;
    case KING:      ch = 'k'; break;
    case NULL_TYPE: ch = ' '; break;
    }

    if (p->colour == (g_setWhiteIsUpper ? WHITE : BLACK))
        ch = toupper(ch);

    return ch;
}

const char* getPiece2Ch(const Piece* p)
{
    assertPiece(p);

    static char str[2] = "";

    str[0] = (p->colour == NULL_COLOUR) ? ' ' : 
             (p->colour == WHITE) ? 'w' : 'b';

    switch (p->type)
    {
    case PAWN:      str[1] = 'p'; break;
    case KNIGHT:    str[1] = 'h'; break;
    case BISHOP:    str[1] = 'b'; break;
    case ROOK:      str[1] = 'r'; break;
    case QUEEN:     str[1] = 'q'; break;
    case KING:      str[1] = 'k'; break;
    case NULL_TYPE: str[1] = ' '; break;
    }

    return str;
}

#endif
