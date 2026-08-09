#include <string.h>
#include "../include/board-output.h"
#include "../include/io-plus.h"
#include "../include/move-rules.h"
#include "../include/chess-game.h"
#include "../include/piece-traits.h"

static bool gProgramIsRunning = true;

void endProgram(void)
{
    gProgramIsRunning = false;
}

void helpCommand(void);

void listCommands(void);

void printCurrChessGame(void)
{
    printBoard(gCurrChessGame.board);
}

void printCurrInits(void)
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
            printf("%d, ",initPositionAt(&gCurrChessGame,x,y));
        printf("\n");
    }
}

const char* getBoardMove(BoardMove* fromBoardMove)
{
    ErrorCode errCodePosX = 0, errCodePosY = 0, errCodeMoveX = 0, errCodeMoveY = 0;

    BoardMove toBoardMove = {
        .position = {getPosNumber(&errCodePosX),7-getPosNumber(&errCodePosY)},
        .move = {getNumber(&errCodeMoveX),-getNumber(&errCodeMoveY)}
    };

    if ((errCodePosX  == INPUT_ERR || errCodePosY  == INPUT_ERR) ||
        (errCodeMoveX == INPUT_ERR || errCodeMoveY == INPUT_ERR))
    {
        clearInput();
        return "Did not move because of failed input\n";
    }
    else if (errCodePosX == NON_POS_ERR || errCodePosY == NON_POS_ERR)
    {
        clearInput();
        return "Position specified has a negative coodinate";
    }

    memcpy(fromBoardMove,&toBoardMove,sizeof(BoardMove));
    return NULL;
}

void moveCommand(void)
{
    BoardMove firstBoardMove = {{0,0},{0,0}};

    const char* inputErrReason = getBoardMove(&firstBoardMove);

    if (inputErrReason != NULL)
    {
        printf("Error: %s",inputErrReason);
        return;
    }

    const MoveErr varOOBErr = getOOBMoveErr(&firstBoardMove);

    if (varOOBErr != NO_MOVE_ERR)
    {
        printf("Error: %s", getMoveErrReason(varOOBErr));
        return;
    }

    clearInput();

    const BoardMove currBoardMove = firstBoardMove;

    const IntVec2D nextPosition = addVecs(&currBoardMove.position,&currBoardMove.move);

    const PieceMove currPieceMove = 
    {
        .mover = getCurrPieceAtVec(&currBoardMove.position),
        .captured = getCurrPieceAtVec(&nextPosition),
    };

    const ChessMove currChessMove = {currBoardMove,currPieceMove};

    const MoveErr mvErr = getMoveErr(&gCurrChessGame,&currChessMove);

    if (mvErr != NO_MOVE_ERR)
    {
        printf("Error: %s",getMoveErrReason(mvErr));
        return;
    }

    movePieceUncond(&gCurrChessGame,&currBoardMove);

    flipCurrChessGame();
    printCurrChessGame();
    gCurrChessGame.player = oppositeColour(gCurrChessGame.player);
}

typedef struct
{
    const char* const name;
    void(*getAndRun)(void);
    const char* const helpText;
} Command;

// This is a placeholder funtion for incomplete commands
void fooCmdFunc(void)
{
    printf("This is a placeholder function!\n");
}

static const Command CMD_LIST[] =
{
    {
        .name = "print",
        .getAndRun = printCurrChessGame,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "flip",
        .getAndRun = flipCurrChessGame,
        .helpText = "Flips the board vertically",
    },

    {
        .name = "restart",
        .getAndRun = resetCurrChessGame,
        .helpText = "Resets the board to its initial state",
    },

    {
        .name = "setup",
        .getAndRun = runSetup,
        .helpText = "Makes a series of questions appear to costumize your chess experience!",
    },

    {
        .name = "quit",
        .getAndRun = endProgram,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "move",
        .getAndRun = moveCommand,
        .helpText = "Moves the pieces using x and y or chess notation",
    },
   
    {
        .name = "clear",
        .getAndRun = clearOutput,
        .helpText = "Clears the output from the console",
    },

    {
        .name = "list",
        .getAndRun = listCommands,
        .helpText = "Lists all avaliable commands",
    },

    {
        .name = "help",
        .getAndRun = helpCommand,
        .helpText = "Displays text about the command",
    },

    { // this is going to be like a null terminator
        .name = "",
    },

};

int indexOfCmd(const char* const cmd)
{
    for (int i = 0; strcmp(CMD_LIST[i].name, "") != 0; i++)
        if (strcmp(cmd, CMD_LIST[i].name) == 0)
            return i;

    printf("Error: %s command does not exist\n", cmd);
    clearInput();
    return -1;
}

void helpCommand(void)
{
    char cmd[MAX_STR_LENGTH] = "";
    strcpy(cmd,getStr());
    if (indexOfCmd(cmd) != -1)
        printf("%s\n", CMD_LIST[indexOfCmd(cmd)].helpText);
}

void listCommands(void)
{
    for (int i = 0; strcmp(CMD_LIST[i].name, "") != 0; i++)
        printf("%s\n", CMD_LIST[i].name);
}

void getAndRunCommand(const char* const cmd)
{
    if (indexOfCmd(cmd) != -1)
        CMD_LIST[indexOfCmd(cmd)].getAndRun();
}

void commandLoop(void)
{
    clearOutput();

    char command[MAX_STR_LENGTH] = "";

    printf("Welcome to minimal-chess!\n"
           "Use \"list\" to list all commands\n"
           "And use \"help [command]\" for text about the command\n");

    resetCurrChessGame();
    printCurrChessGame();

    while (gProgramIsRunning)
    {
        printf(">> ");
        strcpy(command,getStr());
        getAndRunCommand(command);
    }
}
