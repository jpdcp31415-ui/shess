#if __STDC_VERSION__ < 199901L
    #error I need at least C99 to compile
#endif

#include <string.h>
#include "../include/piece-traits.h"
#include "../include/board-output.h"
#include "../include/io-plus.h"

bool gProgramIsRunning = true;

void endProgram(void)
{
    gProgramIsRunning = false;
}

void helpCommand(void);

void listCommands(void);

void moveCurrAt(void)
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

const Command CMD_LIST[] =
{
    {
        .name = "print",
        .getAndRun = printCurrBoard,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "flip",
        .getAndRun = flipCurrBoard,
        .helpText = "Flips the board vertically",
    },

    {
        .name = "restart",
        .getAndRun = resetCurrBoard,
        .helpText = "Resets the board to its initial state",
    },

    {
        .name = "setup",
        .getAndRun = runSetup,
        .helpText = "Makes a series of questions appear to costumize your chess experience!",
    },

    {
        .name = "end",
        .getAndRun = endProgram,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "move",
        .getAndRun = moveCurrAt,
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

void gameLoop(void)
{
    clearOutput();

    char command[MAX_STR_LENGTH] = "";

    printf("Welcome to minimal-chess!\n"
           "Use \"list\" to list all commands\n"
           "And use \"help [command]\" for text about the command\n");

    while (gProgramIsRunning)
    {
        printf("> ");
        strcpy(command,getStr());
        getAndRunCommand(command);
    }
}

int main(void)
{
    gameLoop();
}
