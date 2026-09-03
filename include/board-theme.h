#ifndef BOARD_THEME_H
#define BOARD_THEME_H

#include "piece.h"
#include "board.h"

#define NO_FILL_CHAR '\0'

typedef const char* StrTile;
typedef const char* StrTileArr[];

typedef struct
{
    const int width;
    const int height;
} StrTileSize;

typedef struct
{
    const char** pieceMap;
    const StrTileSize tileSize;
    const char* themeName;
    const char fillChar;
} BoardTheme;

const BoardTheme* getKBoardThemePtr(const char* theme);

StrTile getStrTile(const char* themeName, const Piece* p);

const char* getStrTileRow(const char* themeName, const Piece* p, const int row);

#endif
