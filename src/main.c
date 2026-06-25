#if __STDC_VERSION__ < 199901L
    #error I need at least C99 to compile
#endif

#include <string.h>
#include "../include/piece-traits.h"
#include "../include/chess-grid-output.h"
#include "../include/io-plus.h"

bool gProgramIsRunning = true;

void endProgram(void)
{
    gProgramIsRunning = false;
}

void runSetup(void)
{
    printf("Starting Setup:\n"); 
    
    // Print mode
    printf("Do you want to use Emojis or ASCII (one or two chars) for the pieces? (E/1/2)");
getAgain:
    switch (getSingleChar())
    {
    case '1': gSetPrintMode = ONE_CHAR;  break;
    case '2': gSetPrintMode = TWO_CHARS; break; // this is the classic way
    case 'E': gSetPrintMode = EMOJI;     break;
    default:
        printf("Inputted character is invalid.\nTry again: ");
        goto getAgain;
    }

    // short versions for address of anonymous? variable
    const Piece whitePawn = {WHITE, PAWN};
    const Piece blackPawn = {BLACK, PAWN};

    // Space between pieces    
	printf("Preview with space: ");
    printPiece(&whitePawn); printf(" "); printPiece(&whitePawn);

    printf("\n");

	printf("Preview without space: ");
    printPiece(&whitePawn); printPiece(&whitePawn);

    printf("\n");

    printf("Do you want spaces between the pieces? (Y/n): ");
    gSetSpaceBetween = getYesOrNo();
    
    // Invert colours of emojis
    if (gSetPrintMode == EMOJI)
    {
        printf("Do you want to invert the colours of the pieces (only displaying)\n");
        printf("White pawn: %s, Black pawn: %s\n",
                getPieceEmoji(&whitePawn),
                getPieceEmoji(&blackPawn));
        g_setInvertPieceColours = getYesOrNo();
    }

    printf("Ending Setup\n");  
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
    void(*run)(void);
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
        .run = printCurrBoard,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "flip",
        .run = flipCurrBoard,
        .helpText = "Flips the board vertically",
    },

    {
        .name = "restart",
        .run = resetCurrBoard,
        .helpText = "Resets the board to its initial state",
    },

    {
        .name = "setup",
        .run = runSetup,
        .helpText = "Makes a series of questions appear to costumize your chess experience!",
    },

    {
        .name = "end",
        .run = endProgram,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "move",
        .run = moveCurrAt,
        .helpText = "Moves the pieces using x and y or chess notation",
    },
   
    {
        .name = "clear",
        .run = clearOutput,
        .helpText = "Clears the output from the console",
    },

    {
        .name = "list",
        .run = listCommands,
        .helpText = "Lists all avaliable commands",
    },

    {
        .name = "help",
        .run = helpCommand,
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
        CMD_LIST[indexOfCmd(cmd)].run();
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
