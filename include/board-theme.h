#ifndef BOARD_THEME_H
#define BOARD_THEME_H

#include "settings.h"
#include "assert-toggle.h"
#include "piece.h"
#include "int-vec-2d.h"
#include "board.h"

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
    bool hasFill;
} BoardTheme;

const BoardTheme* getKBoardThemePtr(const char* theme);

StrTile getStrTile(const char* themeName, const Piece* p);

const char* getStrTileRow(const char* themeName, const Piece* p, const int row);

#endif
