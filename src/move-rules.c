#include "../include/io-plus.h"
#include "../include/piece-traits.h"
#include "../include/int-vec-2d.h"
#include "../include/board-output.h"
#include "../include/chess-game.h"
#include "../include/move-rules.h"

typedef IntVec2D IntCart2D; // Cart stands for cartesian coordinates

IntCart2D toCart(const IntVec2D* vec)
{
    return (IntVec2D){vec->x,7-vec->y};
}

typedef struct 
{
    IntCart2D position;
    IntCart2D move;
} BoardMove;

BoardMove getMove(void);

