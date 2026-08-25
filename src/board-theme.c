#include "../include/board-theme.h"
#include <string.h>

const Piece pieceMapOrder[] = {
    {NULL_COLOUR,NULL_TYPE},

    {WHITE,PAWN},
    {WHITE,KNIGHT},
    {WHITE,BISHOP},
    {WHITE,ROOK},
    {WHITE,QUEEN},
    {WHITE,KING},

    {BLACK,PAWN},
    {BLACK,KNIGHT},
    {BLACK,BISHOP},
    {BLACK,ROOK},
    {BLACK,QUEEN},
    {BLACK,KING},
};

const BoardTheme gAllBoardThemes[] =
{
    {
        .themeName = "emoji",
        .tileSize = {1,1},
        .pieceMap = (const StrTile[]) {
            " ",
            "\u2659","\u2658","\u2657","\u2656","\u2655","\u2654",
            "\u265F","\u265E","\u265D","\u265C","\u265B","\u265A"
        },
    },

    {
        .themeName = "1char",
        .tileSize = {1,1},
        .pieceMap = (const StrTile[]) {
            " ",
            "p","n","b","r","q","k",
            "P","N","B","R","Q","K"
        },
    },

    {
        .themeName = "2chars",
        .tileSize = {2,1},
        .pieceMap = (const StrTile[]) {
            "  ",
            "wp","wn","wb","wr","wq","wk",
            "bp","bn","bb","br","bq","bk"
        },
    },

    { // Works as a null terminator
        .themeName = NULL,
    }
};

const BoardTheme* getKBoardThemePtr(const char* themeName)
{
    for (int i = 0; gAllBoardThemes[i].themeName != NULL; i++)
        if (strcmp(gAllBoardThemes[i].themeName,themeName))
            return &gAllBoardThemes[i];
    
    return NULL;
}

int getPieceIndexInMap(const Piece* p)
{
    assertPiece(p);

    for (int i = 0; i < 13; i++)
        if (equalPiece(p,&pieceMapOrder[i]))
            return i;

    EXIT_MSG("How did you get here?");
}

StrTile getStrTile(const char* themeName, const Piece* p)
{
    assertPiece(p);

    const BoardTheme* theme = getKBoardThemePtr(themeName);

    ASSERT_NOT_EQ(theme, NULL, "%p");

    return theme->pieceMap[getPieceIndexInMap(p)];
}

const char* getStrTileRow(const char* themeName, const Piece* p, const int row)
{
    ASSERT_LE(row, getKBoardThemePtr(themeName)->tileSize.y, "%d");

    const char* strTileRow = getStrTile(themeName,p);

    for (int i=1; i < row; i++)
        strTileRow = strchr(strTileRow, '\0');
        
    return strTileRow;
}
