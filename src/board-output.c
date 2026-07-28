#include <stdio.h>
#include "../include/board-output.h"
#include "../include/io-plus.h"

typedef enum
{
    ONE_CHAR,
    TWO_CHARS,
    EMOJI,
} PrintMode;

typedef struct
{
    PrintMode printMode;
    bool invertColours;
    bool whiteIsUpper;
    int emptySpaceAs1or2Ch; // false means 1, true means 2
    bool spaceBetween;
    bool emptySpaceAsUnderscore;
    bool printCoords;
} OutputSettings;

OutputSettings gOutputSettings = 
{
    .printMode = EMOJI,
    .invertColours = true,
    .whiteIsUpper = false,
    .emptySpaceAs1or2Ch = 1,
    .spaceBetween = true,
    .printCoords = true,
};

// unicode character range: \u2654-F
// order: king, queen, rook, bishop, knight, pawn
const char* getDefPieceEmoji(const Piece* p)
{
    assertPiece(p);

    if (isBlankSpace(p))
    {
        if (gOutputSettings.emptySpaceAs1or2Ch)
        {
            if (gOutputSettings.emptySpaceAsUnderscore) return "_";
            else return " ";
        }
        else
        {
            if (gOutputSettings.emptySpaceAsUnderscore) return "__";
            else return "  ";
        }
    }
    
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

const char* getPieceEmoji(const Piece* p)
{
    if (gOutputSettings.invertColours)
        return getDefPieceEmoji(&(const Piece){
                oppositeColour(p->colour),
                p->type});

    return getDefPieceEmoji(p);
}

char getPiece1Ch(const Piece* p)
{
    assertPiece(p);

    static const char pieceChArr[] = " pnbrqk";

    return (p->colour == (gOutputSettings.whiteIsUpper ? WHITE : BLACK)) ?
            toupper(pieceChArr[p->type]) : pieceChArr[p->type];
}

const char* getPiece2Ch(const Piece* p)
{
    assertPiece(p);

    static char str[3] = "";

    str[0] = (p->colour == NULL_COLOUR) ? ' ' : 
             (p->colour == WHITE) ? 'w' : 'b';

    switch (p->type)
    {
    case PAWN:      str[1] = 'p'; break;
    case KNIGHT:    str[1] = 'n'; break;
    case BISHOP:    str[1] = 'b'; break;
    case ROOK:      str[1] = 'r'; break;
    case QUEEN:     str[1] = 'q'; break;
    case KING:      str[1] = 'k'; break;
    case NULL_TYPE: str[1] = ' '; break;
    }

    return str;
}

void printPiece(const Piece* p)
{
    switch (gOutputSettings.printMode)
    {
    case ONE_CHAR:  printf("%c", getPiece1Ch(p));   break;
    case TWO_CHARS: printf("%s", getPiece2Ch(p));   break;
    case EMOJI:     printf("%s", getPieceEmoji(p)); break;
    }
}

void printBoard(Board grid)
{
    for (int y=0; y<8; y++)
    {
        if (gOutputSettings.printCoords)
            printf("%d ", 7-y);
        for (int x=0; x<8; x++)
        {
            printPiece(getPiecePtrAt(grid,x,y));
            if (gOutputSettings.spaceBetween)
                printf(" ");
        }
        printf("\n");
    }

    if (gOutputSettings.printCoords)
    {
        printf("  ");
        for (int x=0; x<8; x++)
        {
            printf("%d", x);
            if (gOutputSettings.spaceBetween)
                printf(" ");
        }
        printf("\n");
    }
}

char getAndMatchCh(const char* const matchChs)
{
    const char inputtedCh = getSingleChar();

    while (true)
    {
        for (int i = 0; matchChs[i] != '\0'; i++)
            if (tolower(inputtedCh) == matchChs[i] ||
                toupper(inputtedCh) == matchChs[i])
                return inputtedCh;
        printf("Error: %c is not a valid character. Try again: ", inputtedCh);
    }
}
/* remember to use this function /\/\/\ */

void runSetup(void)
{
    printf("Starting Setup:\n"); 
    
    // Print mode
    printf("Do you want to use Emojis or ASCII (one or two chars) for the pieces? (E/1/2): ");
getAgain:
    switch (getSingleChar())
    {
    case '1': gOutputSettings.printMode = ONE_CHAR;  break;
    case '2': gOutputSettings.printMode = TWO_CHARS; break; // this is the classic way
    case 'E': gOutputSettings.printMode = EMOJI;     break;
    default:
        printf("Inputted character is invalid.\nTry again: ");
        goto getAgain;
    }

    // short versions for address of anonymous? variable
    const Piece whitePawn = {WHITE, PAWN};
    const Piece blackPawn = {BLACK, PAWN};

    // Invert colours of emojis
    if (gOutputSettings.printMode == EMOJI)
    {
        printf("Do you want to invert the colours of the pieces (only displaying)\n");
        printf("White pawn: %s, Black pawn: %s\n",
                getPieceEmoji(&whitePawn),
                getPieceEmoji(&blackPawn));
        gOutputSettings.invertColours = getYesOrNo();

        printf("How many spaces do you want for an empty space to be displayed?: ");

        ErrorCode errCode = NO_ERRS;

        while (true)
        {
            gOutputSettings.emptySpaceAs1or2Ch = getPosNumber(&errCode);

            if (errCode == NON_POS_ERR)
            {
                clearInput();
                printf("Error: Number inputted is not positive: ");
            }
            else if (errCode == INPUT_ERR)
            {
                clearInput();
                printf("Error: Input didn't go well. Try again: ");
            }
            else if (!(gOutputSettings.emptySpaceAs1or2Ch == 1 ||
                       gOutputSettings.emptySpaceAs1or2Ch == 2))
            {
                printf("Number of spaces can only be 1 or 2. Try again: ");
                clearInput();
            }
            else
                break;
        }
    }

    // Space between pieces    
	printf("Preview with space: ");
    printPiece(&whitePawn); printf(" "); printPiece(&whitePawn);

    printf("\n");

	printf("Preview without space: ");
    printPiece(&whitePawn); printPiece(&whitePawn);

    printf("\n");

    printf("Do you want spaces between the pieces? (Y/n): ");
    gOutputSettings.spaceBetween = getYesOrNo();
    
    // Display empty spaces as underscores
    printf("Do you want empty spaces to be displayed as \"_\" (y/N): ");
    gOutputSettings.emptySpaceAsUnderscore = getYesOrNo();

    printf("Do you want for coordinates to be displayed? (Y/n): ");
    gOutputSettings.printCoords = getYesOrNo();

    printf("Ending Setup\n");  
}
