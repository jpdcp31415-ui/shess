#include <stdio.h>
#include <string.h>

#include "../include/board-output.h"
#include "../include/io-plus.h"
#include "../include/assert-toggle.h"
#include "../include/board-theme.h"
#include "../include/settings.h"

void printBoardRowWithTheme(const char* themeName, const Board board, const int y)
{
    const BoardTheme* themePtr = getKBoardThemePtr(themeName);

    if (getSettingData("show-board-coords").boolData)
        printf("%d ", 7-y);

    for (int row = 0; row < themePtr->tileSize.y; row++)
    {
        for (int x=0; x<8; x++)
        {
            const char* rowTileStr = getStrTileRow(getSettingData("board-theme").strData,
                                                   getKPiecePtrAt(board,x,y),
                                                   row);
            printf("%s", rowTileStr);
            if (getSettingData("space-between-columns").boolData)
                printf(" ");
        }
        printf("\n");
    }
}

void printBoardWithTheme(const char* themeName, const Board board)
{
    for (int y=0; y<8; y++)
        printBoardRowWithTheme(themeName,board,y);

    if (getSettingData("show-board-coords").boolData)
    {
        printf("  ");
        for (int x=0; x<8; x++)
        {
            printf("%d", x);
            if (getSettingData("space-between-columns").boolData)
                printf(" ");
        }
        printf("\n");
    }
}

void printBoard(const Board board)
{
    printBoardWithTheme(getSettingData("board-theme").strData,board);
}

char getAndMatchCh(const char* const matchChs)
{
    const char inputtedCh = getSingleChar();

    while (true)
    {
        if (strchr(matchChs,tolower(inputtedCh)) ||
            strchr(matchChs,toupper(inputtedCh)))
            return inputtedCh;

        printf("Error: %c is not a valid character. Try again: ", inputtedCh);
    }
}
/* remember to use this function /\/\/\ */

/* void runSetup(void) */
/* { */
/*     printf("Starting Setup:\n");  */
/*      */
/*     // Print mode */
/*     printf("Do you want to use Emojis or ASCII (one or two chars) for the pieces? (E/1/2): "); */
/* getAgain: */
/*     switch (getSingleChar()) */
/*     { */
/*     case '1': gOutputSettings.printMode = ONE_CHAR;  break; */
/*     case '2': gOutputSettings.printMode = TWO_CHARS; break; // this is the classic way */
/*     case 'E': gOutputSettings.printMode = EMOJI;     break; */
/*     default: */
/*         printf("Inputted character is invalid.\nTry again: "); */
/*         goto getAgain; */
/*     } */
/*  */
/*     const Piece whitePawn = {WHITE, PAWN}; */
/*     const Piece blackPawn = {BLACK, PAWN}; */
/*  */
/*     // Invert colours of emojis */
/*     if (gOutputSettings.printMode == EMOJI || gOutputSettings.printMode == ONE_CHAR) */
/*     { */
/*         printf("Do you want to invert the colours of the pieces (only displaying)\n"); */
/*  */
/*         printf("White pawn: "); */
/*                 printPiece(&whitePawn), */
/*         printf(", Black pawn: "); */
/*                 printPiece(&blackPawn), */
/*         printf("\n"); */
/*  */
/*         gOutputSettings.invertColours = getYesOrNo(); */
/*     } */
/*  */
/*     if (gOutputSettings.printMode == EMOJI) */
/*     { */
/*         printf("How many spaces do you want for an empty space to be displayed?: "); */
/*  */
/*         ErrorCode errCode = NO_ERRS; */
/*  */
/*         while (true) */
/*         { */
/*             gOutputSettings.emptySpaceAs1or2Ch = getPosNumber(&errCode); */
/*  */
/*             if (errCode == NON_POS_ERR) */
/*             { */
/*                 clearInput(); */
/*                 printf("Error: Number inputted is not positive: "); */
/*             } */
/*             else if (errCode == INPUT_ERR) */
/*             { */
/*                 clearInput(); */
/*                 printf("Error: Input didn't go well. Try again: "); */
/*             } */
/*             else if (!(gOutputSettings.emptySpaceAs1or2Ch == 1 || */
/*                        gOutputSettings.emptySpaceAs1or2Ch == 2)) */
/*             { */
/*                 printf("Number of spaces can only be 1 or 2. Try again: "); */
/*                 clearInput(); */
/*             } */
/*             else break; */
/*         } */
/*     } */
/*  */
/*     // Space between pieces     */
/* 	printf("Preview with space: "); */
/*     printPiece(&whitePawn); printf(" "); printPiece(&whitePawn); */
/*  */
/*     printf("\n"); */
/*  */
/* 	printf("Preview without space: "); */
/*     printPiece(&whitePawn); printPiece(&whitePawn); */
/*  */
/*     printf("\n"); */
/*  */
/*     printf("Do you want spaces between the pieces? (Y/n): "); */
/*     gOutputSettings.spaceBetween = getYesOrNo(); */
/*      */
/*     // Display empty spaces as underscores */
/*     printf("Do you want empty spaces to be displayed as \"_\" (y/N): "); */
/*     gOutputSettings.emptySpaceAsUnderscore = getYesOrNo(); */
/*  */
/*     printf("Do you want for coordinates to be displayed? (Y/n): "); */
/*     gOutputSettings.printCoords = getYesOrNo(); */
/*  */
/*     printf("Ending Setup\n");   */
/* } */
