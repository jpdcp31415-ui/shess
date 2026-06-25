#include <stdio.h>
#include "../include/chess-grid-output.h"

PrintMode gSetPrintMode = EMOJI;

void printPiece(const Piece* p)
{
    switch (gSetPrintMode)
    {
    case ONE_CHAR:  printf("%c", getPiece1Ch(p));   break;
    case TWO_CHARS: printf("%s", getPiece2Ch(p));   break;
    case EMOJI:     printf("%s", getPieceEmoji(p)); break;
    }
}

bool gSetSpaceBetween = true;

void printBoard(ChessGrid grid)
{
    for (int y=0; y<8; y++)
    {
        for (int x=0; x<8; x++)
        {
            printPiece(getPieceAt(grid,x,y));
            if (gSetSpaceBetween)
                printf(" ");
        }
        printf("\n");
    }
}

void printCurrBoard(void)
{
    printBoard(gCurrBoard);
}
