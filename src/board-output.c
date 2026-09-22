#include <stdio.h>
#include <string.h>

#include "../include/board-output.h"
#include "../include/io-plus.h"
#include "../include/board-theme.h"
#include "../include/settings.h"

void printBoardRowWithTheme(const char* themeName, const Board board, const int y)
{
    const BoardTheme* themePtr = getKBoardThemePtr(themeName);

    if (getSettingDataSafely("show-board-coords").boolData)
        printf("%d ", 7-y);

    for (int row = 0; row < themePtr->tileSize.height; row++)
    {
        if (getSettingDataSafely("show-board-coords").boolData && row != 0)
            printf("  ");

        for (int x=0; x<8; x++)
        {
            const char* rowTileStr = getStrTileRow(getSettingDataSafely("board-theme").strData,
                                                   getKPiecePtrAt(board,x,y),
                                                   row);
            printf("%s", rowTileStr);
            if (getSettingDataSafely("space-between-columns").boolData)
                printf(" ");
        }
        printf("\n");
    }
}

void printBoardWithTheme(const char* themeName, const Board board)
{
    for (int y=0; y<8; y++) printBoardRowWithTheme(themeName,board,y);

    const BoardTheme* themePtr = getKBoardThemePtr(themeName);

    if (getSettingDataSafely("show-board-coords").boolData)
    {
        printf("  ");
        for (int x=0; x<8; x++)
        {
            printf("%d", x);
            if (getSettingDataSafely("space-between-columns").boolData)
                for (int col = 0; col < themePtr->tileSize.width; col++)
                    printf(" ");
        }
        printf("\n");
    }
}

void printBoard(const Board board)
{
    printBoardWithTheme(getSettingDataSafely("board-theme").strData,board);
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
