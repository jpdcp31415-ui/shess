#include "../include/io-plus.h"
#include "../include/piece-traits.h"
#include "../include/int-vec-2d.h"
#include "../include/board-output.h"

typedef IntVec2D IntCart2D;

typedef struct 
{
    IntCart2D position;
    IntCart2D move;
} BoardMove;

BoardMove getMove(void);

void moveCommand(void)
{
    ErrorCode errCodePosX = 0, errCodePosY = 0, errCodeMoveX = 0, errCodeMoveY = 0;
    const IntVec2D position = {getPosNumber(&errCodePosX),7-getPosNumber(&errCodePosY)};

    if ((errCodePosX == INPUT_ERR  || errCodePosY  == INPUT_ERR) ||
        (errCodeMoveX == INPUT_ERR || errCodeMoveY == INPUT_ERR))
    {
        printf("Did not move because of failed input\n");
        clearInput();
        return;
    }
    else if (errCodePosX == NON_POS_ERR || errCodePosY == NON_POS_ERR)
    {
        printf("Error: position specified has a negative coodinate");
        clearInput();
        return;
    }
    else if (!isVecInBoardBounds(&position))
    {
        printf("Error: position specified is out of bounds\n");
        clearInput();
        return;
    }

    const IntVec2D move     = {getNumber(&errCodeMoveX),getNumber(&errCodeMoveY)};
    const IntVec2D nextPostion = addVecs(&position, &move);

    if (!isVecInBoardBounds(&nextPostion))
    {
        printf("Error: position + move specified is out of bounds\n");
        clearInput();
        return;
    }

    clearInput();

    const Piece movingPiece = *getCurrPieceAtVec(&position);

    if (!hasMove(&move, &movingPiece))
    {
        printf("Move is not valid for this piece\n");
        return;
    }

    *getCurrPieceAtVec(&nextPostion) = *getCurrPieceAtVec(&position);
    *getCurrPieceAtVec(&position) = (Piece){NULL_COLOUR, NULL_TYPE};
}
