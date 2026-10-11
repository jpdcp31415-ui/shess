#include <stdio.h>
#include <string.h>

#include "../include/board-output.h"
#include "../include/io-plus.h"
#include "../include/board-theme.h"
#include "../include/settings.h"
#include "../include/colour-escapes.h"

typedef struct
{
    const char* theme;
    const TermRgbColour colour;
    const bool showCoords;
    const bool spaceBetweenColumns;
} BoardOutputSettings;

void printBoardRowWThemeAndCol(const Board board, const BoardOutputSettings* settings, const int y)
{
    const BoardTheme* themePtr = getKBoardThemePtr(settings->theme);

    if (settings->showCoords)
        printf("%d ", 7-y);

    for (int row = 0; row < themePtr->tileSize.height; row++)
    {
        if (settings->spaceBetweenColumns && row != 0)
            printf("  ");
        
        printRgbColEsc(&settings->colour);
        for (int x=0; x<8; x++)
        {
            const char* rowTileStr = getStrTileRow(getSettingDataSafely("board-theme").strData,
                                                   getKPiecePtrAt(board,x,y),
                                                   row);
            printf("%s", rowTileStr);
            if (settings->spaceBetweenColumns)
                printf(" ");
        }
        printf(ESC_NORMAL_FMT);
        printf("\n");
    }
}

void printBoardWThemeAndCol(const Board board, const BoardOutputSettings* settings)
{
    for (int y=0; y<8; y++) printBoardRowWThemeAndCol(board, settings, y);
    const BoardTheme* themePtr = getKBoardThemePtr(settings->theme);
    if (getSettingDataSafely("show-board-coords").boolData)
    {
        printf("  ");
        for (int x=0; x<8; x++)
        {
            printf("%d", x);
            if (settings->spaceBetweenColumns)
                for (int col = 0; col < themePtr->tileSize.width; col++)
                    printf(" ");
        }
        printf("\n");
    }
}

void printBoard(const Board board)
{
    const BoardOutputSettings settings = {
        .theme = getSettingDataSafely("board-theme").strData,
        .colour = getSettingDataSafely("board-colour").colourData,
        .spaceBetweenColumns = getSettingDataSafely("space-between-columns").boolData,
        .showCoords = getSettingDataSafely("show-board-coords").boolData,
    };

    printBoardWThemeAndCol(
        board,
        &settings
    );
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
