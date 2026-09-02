#include "../include/board-theme.h"
#include "../include/settings.h"

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
        .pieceMap = (StrTileArr) {
            (StrTile){" "},
            (StrTile){"\u265F"},(StrTile){"\u265E"},(StrTile){"\u265D"},
            (StrTile){"\u265C"},(StrTile){"\u265B"},(StrTile){"\u265A"},
            (StrTile){"\u2659"},(StrTile){"\u2658"},(StrTile){"\u2657"},
            (StrTile){"\u2656"},(StrTile){"\u2655"},(StrTile){"\u2654"},
        },
        .hasFill = false
    },

    {
        .themeName = "1char",
        .tileSize = {1,1},
        .pieceMap = (StrTileArr) {
            (StrTile){" "},
            (StrTile){"p"},(StrTile){"n"},(StrTile){"b"},(StrTile){"r"},(StrTile){"q"},(StrTile){"k"},
            (StrTile){"P"},(StrTile){"N"},(StrTile){"B"},(StrTile){"R"},(StrTile){"Q"},(StrTile){"K"},
        },
        .hasFill = false
    },

    {
        .themeName = "2chars",
        .tileSize = {2,1},
        .pieceMap = (StrTileArr) {
            (StrTile){"  "},
            (StrTile){"wp"},(StrTile){"wn"},(StrTile){"wb"},(StrTile){"wr"},(StrTile){"wq"},(StrTile){"wk"},
            (StrTile){"bp"},(StrTile){"bn"},(StrTile){"bb"},(StrTile){"br"},(StrTile){"bq"},(StrTile){"bk"},
        },
        .hasFill = false
    },

    {
        .themeName = "good",
        .tileSize = {5,4},
        .pieceMap = (StrTileArr) {
            (StrTile){
                "     ",
                "     ",
                "     ",
                "     "
            },

            (StrTile){  // 'f' is a char for filling in
                        // with the white/black char from settings
                " (f) ",
                "  $  ",
                " (f) ",
                "(fff)"
            },

            (StrTile){
                "  _^ ",
                " /of|",
                "%fff|",
                " |ff|"
            },

            (StrTile){
                "  o  ",
                " (/) ",
                " (f) ",
                "(fff)"
            },
            
            (StrTile){
                "U U U",
               "\\fff/",
                " |f| ",
                "/fff\\"
            },

            (StrTile){
                "o O o",
               "\\_|_/",
                " )f( ",
                "(fff)"
            },

            (StrTile){
                "  +  ",
               "/\\|/\\",
               "\\fff/",
                " fff "
            },
        },
        .hasFill = true
    },

    { // Works as a null terminator
        .themeName = NULL,
    }
};

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

StrTileRet getStrTile(const char* themeName, const Piece* p)
{
    assertPiece(p);

    const BoardTheme* theme = getKBoardThemePtr(themeName);

    ASSERT_NOT_EQ(theme, NULL, "%p");

    return theme->hasFill && p->colour == BLACK ?
           theme->pieceMap[getPieceIndexInMap(p) - 6] :
           theme->pieceMap[getPieceIndexInMap(p)];
}

void replaceFillChar(char* strTileRow, const Piece* p)
{
    while (strchr(strTileRow,'f') != NULL)
        *strchr(strTileRow, 'f') = p->colour == WHITE ?
                                   getSettingData("white-fill-char").charData :
                                   getSettingData("black-fill-char").charData;
}

const char* getStrTileRow(const char* themeName, const Piece* p, const int row)
{
    ASSERT_LE(row, getKBoardThemePtr(themeName)->tileSize.y, "%d");

    const char* strTileRow = getStrTile(themeName,p)[row];

    static char strTileRowCopy[256] = "";

    strcpy(strTileRowCopy, strTileRow);

    if (strchr(strTileRow, 'f') != NULL)
        replaceFillChar(strTileRowCopy, p);
    
    return strTileRowCopy;
}
