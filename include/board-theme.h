#include "assert-toggle.h"
#include "piece.h"
#include "int-vec-2d.h"

typedef const char* StrTile;

typedef IntVec2D StrTileSize;

typedef struct
{
    const StrTile* pieceMap;
    const StrTileSize tileSize;
    const char* themeName;
} BoardTheme;

const BoardTheme* getKBoardThemePtr(const char* theme);

StrTile getStrTile(const char* themeName, const Piece* p);

const char* getStrTileRow(const char* themeName, const Piece* p, const int row);

void printPiece(const Piece*);

void printChessBoard(Piece board[8][8]);
