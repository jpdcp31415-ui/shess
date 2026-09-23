#include "../include/board-theme.h"
#include "../include/settings.h"
#include "../include/assert-toggle.h"
#include "../include/settings.h"

#include <stdio.h>
#include <string.h>

const Piece pieceMapOrder[] = {
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
        .tileSize = {.width=1, .height=1},
        .pieceMap = (StrTileArr) {
            "\u265F","\u265E","\u265D","\u265C","\u265B","\u265A",
            "\u2659","\u2658","\u2657","\u2656","\u2655","\u2654",
        },
        .fillChar = NO_FILL_CHAR
    },

    {
        .themeName = "1char",
        .tileSize = {.width=1, .height=1},
        .pieceMap = (StrTileArr) {
            "p","n","b","r","q","k",
            "P","N","B","R","Q","K",
        },
        .fillChar = NO_FILL_CHAR
    },

    {
        .themeName = "2chars",
        .tileSize = {.width=2, .height=1},
        .pieceMap = (StrTileArr) {
            "wp","wn","wb","wr","wq","wk",
            "bp","bn","bb","br","bq","bk",
        },
        .fillChar = NO_FILL_CHAR
    },

    {
        .themeName = "good",
        .tileSize = {.width=5, .height=4},
        .pieceMap = (StrTileArr) {
                " (f) \n"
                "  $  \n"
                " (f) \n"
                "(fff)\n",

                "  _^ \n"
                " /of|\n"
                "%fff|\n"
                " |ff|\n",

                "  o  \n"
                " (/) \n"
                " (f) \n"
                "(fff)\n",
            
                "U U U\n"
               "\\fff/\n"
                " |f| \n"
                "/fff\\\n",

                "o O o\n"
               "\\_|_/\n"
                " )f( \n"
                "(fff)\n",

                "  +  \n"
               "/\\|/\\\n"
               "\\fff/\n"
                " fff \n",
        },
        .fillChar = 'f'
    },

    { // Works as a null terminator
        .themeName = NULL,
    }
};

void printBoardThemes(void)
{
    for (int i = 0; gAllBoardThemes[i].themeName != NULL; i++)
        printf("%s\n", gAllBoardThemes[i].themeName);
}

const BoardTheme* getKBoardThemePtr(const char* themeName)
{
    for (int i = 0; gAllBoardThemes[i].themeName != NULL; i++)
        if (strcmp(gAllBoardThemes[i].themeName,themeName) == 0)
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

    return theme->fillChar != NO_FILL_CHAR && p->colour == BLACK ?
           theme->pieceMap[getPieceIndexInMap(p) - 6] :
           theme->pieceMap[getPieceIndexInMap(p)];
}

void replaceFillChar(const char* themeName, char* strTileRow,
                     const char whiteFillCh,
                     const char blackFillCh,
                     const Piece* p)
{
    const BoardTheme* theme = getKBoardThemePtr(themeName);
    while (strchr(strTileRow, theme->fillChar) != NULL)
        *strchr(strTileRow, theme->fillChar) =
            p->colour == WHITE ? whiteFillCh : blackFillCh;
}

const char* getFilledStrOfCh(const char ch, const int len)
{
    static char str[64] = "";
    ASSERT_FMT(len >= 0 && len < 64, "Cannot create string of %d chars", len);
    strcpy(str,""); // clear string because of being static

    for (int i=0; i<len; i++)
        str[i] = ch;

    return str;
}

const char* getStrTileRow(const char* themeName, const Piece* p, const int row)
{
    const BoardTheme* theme = getKBoardThemePtr(themeName);

    ASSERT_FMT(row >= 0 && row < theme->tileSize.height, "Cannot acess row %d", row);

    if (isBlankSpace(p))
        return getFilledStrOfCh(
                getSettingData("blank-fill-char").charData,
                theme->tileSize.width
               );
    
    const StrTile tile = getStrTile(themeName, p);
    static char tileCpy[64] = "";
    strcpy(tileCpy,tile);
    
    char *token = tileCpy;

    if ((token = strtok(tileCpy, "\n")) != NULL)
        for (int i=0; i<row; i++)
            token = strtok(NULL, "\n");

    if (theme->fillChar != NO_FILL_CHAR)
        replaceFillChar(themeName, token, 
                getSettingData("white-fill-char").charData,
                getSettingData("black-fill-char").charData,
                p);
    
    return token;
}
