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
        .tileSize = {.width=1, .height=1},
        .pieceMap = (StrTileArr) {
            " ",
            "\u265F","\u265E","\u265D","\u265C","\u265B","\u265A",
            "\u2659","\u2658","\u2657","\u2656","\u2655","\u2654",
        },
        .hasFill = false
    },

    {
        .themeName = "1char",
        .tileSize = {.width=1, .height=1},
        .pieceMap = (StrTileArr) {
            " ",
            "p","n","b","r","q","k",
            "P","N","B","R","Q","K",
        },
        .hasFill = false
    },

    {
        .themeName = "2chars",
        .tileSize = {.width=2, .height=1},
        .pieceMap = (StrTileArr) {
            "  ",
            "wp","wn","wb","wr","wq","wk",
            "bp","bn","bb","br","bq","bk",
        },
        .hasFill = false
    },

    {
        .themeName = "good",
        .tileSize = {.width=5, .height=4},
        .pieceMap = (StrTileArr) {
                "     \n"
                "     \n"
                "     \n"
                "     \n",

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

StrTile getStrTile(const char* themeName, const Piece* p)
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
    const BoardTheme* theme = getKBoardThemePtr(themeName);
    ASSERT(row >= 0 && row < theme->tileSize.height, "Cannot acess row out of bounds!");
    
    const StrTile tile = getStrTile(themeName, p);
    static char tileCpy[64] = "";
    strcpy(tileCpy,tile);
    
    char *token = tileCpy;

    if ((token = strtok(tileCpy, "\n")) != NULL)
        for (int i=0; i<row; i++)
            token = strtok(NULL, "\n");

    replaceFillChar(token, p);
    
    return token;
}
