#include "settings.h"
#include "assert-toggle.h"
#include "piece.h"
#include "int-vec-2d.h"
#include "board.h"

typedef const char* StrTile[];
typedef const char** StrTileRet;
typedef const char** StrTileArr[];

typedef IntVec2D StrTileSize;

typedef struct
{
    const char*** pieceMap;
    const StrTileSize tileSize;
    const char* themeName;
} BoardTheme;

const BoardTheme* getKBoardThemePtr(const char* theme);

StrTileRet getStrTile(const char* themeName, const Piece* p);

const char* getStrTileRow(const char* themeName, const Piece* p, const int row);
