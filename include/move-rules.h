#ifndef MOVE_RULES_H
#define MOVE_RULES_H

#include "../include/int-vec-2d.h"
#include "../include/chess-game.h"

// new chess move

typedef struct 
{
    IntVec2D position;
    IntVec2D move;
} BoardMove;

typedef struct
{
    Piece mover;
    Piece captured;
} PieceMove;

typedef struct 
{
    BoardMove boardMove;
    PieceMove pieceMove;
} ChessMove;

typedef enum
{
    NO_MOVE_ERR,

    /* OOB is for Out Of Bounds */
    OOB_POSITION,
    OOB_MOVE,

    MOVER_IS_BLANK,
    OPPOSITE_PLAYER_MOVE,
    DOESNT_HAVE_MOVE,

    NO_PIECE_TO_ATTACK, // When pawn is moving diagonally
    NOT_INIT_POSITION, // Moving pawn 2 spaces forward 
    CANT_ATTACK_FRONT,
    PATH_NOT_CLEAR,

    // MOVE CONDITIONS FOR CHECK AND CHECKMATE
    // CANT_MOVE_KING, 
    // CANT_BLOCK_CHECK, 
    // CANT_CAPTURE_OPP_PIECE,?

    /* SPECIAL MOVES */
    CASTLING_CHECK,
    CANT_EN_PASSENT,

    IS_STILL_CHECK,
    MOVE_CAUSES_CHECK,

    SAME_PLAYER_ATTACK,
} MoveErr;

const ChessGame* movedKChessGamePtr(const ChessGame* game, const BoardMove* boardMove);

PieceMove getAsPieceMove(const Board board, const BoardMove* boardMove);

IntVec2D getPawnPromotionPosition(const ChessGame* game);

bool isPawnPromotion(const ChessGame* game);

const char* getMoveErrReason(const MoveErr mvErr);

MoveErr getOOBMoveErr(const BoardMove* boardMove);

bool isWinForOppPlayer(const ChessGame* game);

void movePieceUncond(ChessGame* game, const BoardMove* boardMove);

MoveErr getMoveErr(const ChessGame* game, const ChessMove* chessMove);

bool isValidMove(const ChessGame* game, const ChessMove* chessMove);

#endif
