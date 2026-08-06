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

void moveCommand(void)
{
    ErrorCode errCodePosX = 0, errCodePosY = 0, errCodeMoveX = 0, errCodeMoveY = 0;

    ChessMove currChessMove = {
        .position = {getPosNumber(&errCodePosX),7-getPosNumber(&errCodePosY)},
        .move = {getNumber(&errCodeMoveX),-getNumber(&errCodeMoveY)}
    };

    if ((errCodePosX  == INPUT_ERR || errCodePosY  == INPUT_ERR) ||
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
    else if (!isVecInBoardBounds(&currChessMove.position))
    {
        printf("Error: position specified is out of bounds\n");
        clearInput();
        return;
    }

    const IntVec2D nextPosition = addVecs(&currChessMove.position,&currChessMove.move);

    if (!isVecInBoardBounds(&nextPosition))
    {
        printf("Error: position + move specified is out of bounds\n");
        clearInput();
        return;
    }

    clearInput();

    const Piece pieceAtPosition = getCurrPieceAtVec(&currChessMove.position);

    if (isBlankSpace(&pieceAtPosition))
    {
        printf("Cannot move empty space\n");
        return;
    }
    else if (gCurrChessGame.player == oppositeColour(pieceAtPosition.colour))
    {
        printf("Cannot move piece from opposite player\n");
        return;
    }

    if (!hasMove(&currChessMove.move, &pieceAtPosition))
    {
        printf("Piece does not have this move!\n");
        return;
    }

    const MoveErr mvErr = getMoveErr(&gCurrChessGame,&currChessMove);
    if (mvErr != NO_MOVE_ERR)
    {
        printf("%s",getMoveErrReason(mvErr));
        return;
    }

    const Piece pieceAtNextPosition = getCurrPieceAtVec(&nextPosition);

    if (gCurrChessGame.player == pieceAtNextPosition.colour)
    {
        printf("You cannot attack your own pieces!\n");
        return;
    }

    movePieceUncond(&gCurrChessGame,&currChessMove);

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
