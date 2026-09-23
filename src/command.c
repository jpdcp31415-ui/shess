#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

#include "../include/command.h"
#include "../include/board-output.h"
#include "../include/board-theme.h"
#include "../include/io-plus.h"
#include "../include/move-rules.h"
#include "../include/chess-game.h"
#include "../include/game-state.h"
#include "../include/settings.h"
#include "../include/assert-toggle.h"

void promptForPawnPromotion(ChessGame* game)
{
    bool inputFailed = true;
    while (!inputFailed)
    {
        printf("Pawn promotion (n/b/r/q)?:");
        const char pieceChChosen = getSingleChar();
        Piece pieceChosen = {NULL_COLOUR,NULL_TYPE};

        switch (pieceChChosen)
        {
        case 'n': pieceChosen = (Piece){gCurrChessGame.player, KNIGHT}; break;
        case 'b': pieceChosen = (Piece){gCurrChessGame.player, BISHOP}; break;
        case 'r': pieceChosen = (Piece){gCurrChessGame.player, ROOK};   break;
        case 'q': pieceChosen = (Piece){gCurrChessGame.player, QUEEN};  break;
        default:
            printf("Please enter another character\n");
            inputFailed = true;
        }

        const IntVec2D promPos = getPawnPromotionPosition(game);
        *getPiecePtrAtVec(game->board,&promPos) = pieceChosen;

        break;
    }    
}

void moveCommand(const char* input, const char* usage, const int numArgs)
{
    if (isWinForOppPlayer(&gCurrChessGame))
    {
        printf("Yay! %s won the game!", gCurrChessGame.player == WHITE ? "WHITE" : "BLACK");
        return;
    }

    BoardMove firstBoardMove = {{0,0},{0,0}};

    const int n = sscanf(input, usage,
            &firstBoardMove.position.x,
            &firstBoardMove.position.y,
            &firstBoardMove.move.x,
            &firstBoardMove.move.y);

    if (n != numArgs)
    {
        printf("Error: Input did not go as expected!\n");
        return;
    }

    firstBoardMove.position.y = 7-firstBoardMove.position.y;
    firstBoardMove.move.y = -firstBoardMove.move.y;

    const MoveErr varOOBErr = getOOBMoveErr(&firstBoardMove);

    if (varOOBErr != NO_MOVE_ERR)
    {
        printf("Error: %s", getMoveErrReason(varOOBErr));
        return;
    }

    const BoardMove currBoardMove = firstBoardMove;

    const ChessMove currChessMove = {
        currBoardMove,
        getAsPieceMove(gCurrChessGame.board,&currBoardMove)
    };

    if (!isValidMove(&gCurrChessGame,&currChessMove))
    {
        const MoveErr mvErr = getMoveErr(&gCurrChessGame,&currChessMove);
        printf("Error: %s",getMoveErrReason(mvErr));
        return;
    }

    movePieceUncond(&gCurrChessGame,&currBoardMove);

    if (isPawnPromotion(&gCurrChessGame)) promptForPawnPromotion(&gCurrChessGame);

    flipCurrChessGame();
    printCurrChessGame();
    gCurrChessGame.player = oppositeColour(gCurrChessGame.player);
}

void helpCommand(const char* input, const char* usage, const int numArgs)
{
    char cmd[MAX_CMD_LENGTH] = "";

    const int n = sscanf(input, usage, cmd);

    if (n != numArgs)
    {
        printf("Error: Input did not go as expected!\n");
        return;
    }

    const Command* cmdPtr = findCmdPtr(cmd);
    if (cmdPtr != NULL)
    {
        printf("Usage: %s %s\n", cmd, cmdPtr->argNames);
        printf("Help:  %s\n", cmdPtr->helpText);
        return;
    }

    const NoArgsCommand* noArgsCmdPtr = findNoArgsCmdPtr(cmd);
    if (noArgsCmdPtr != NULL)
    {
        printf("Usage: %s\n", cmd);
        printf("Help:  %s\n", noArgsCmdPtr->helpText);
        return;
    }

    printf("Error: Command %s does not exist!\n", cmd);
}

void printCommands(void)
{
    for (int i = 0; kNoArgsCmdList[i].name != NULL; i++)
    {
        const NoArgsCommand* cmd = &kNoArgsCmdList[i];
        if (strcmp(cmd->name,cmd->shortName) != 0)
            printf("%s / %s\n", cmd->name, cmd->shortName);
        else
            printf("%s\n", cmd->name);
    }

    for (int i = 0; kCmdList[i].name != NULL; i++)
    {
        const Command* cmd = &kCmdList[i];
        if (strcmp(cmd->name,cmd->shortName) != 0)
            printf("%s / %s [%s]\n", cmd->name, cmd->shortName, cmd->argNames);
        else
            printf("%s [%s]\n", cmd->name, cmd->argNames);
    }
}

typedef struct {
    const char* name;
    void(*const printValues)(void);
} ListCategory;

void printCategories(void);

const ListCategory kListCategories[] =
{
    {
        .name = "board-themes",
        .printValues = printBoardThemes,
    },
    {
        .name = "settings",
        .printValues = printSettings,
    },
    {
        .name = "commands",
        .printValues = printCommands,
    },
    {
        .name = "categories",
        .printValues = printCategories,
    },
    { // null terminator
        .name = NULL,
    },
};

void printCategories(void)
{
    for (int i = 0; kListCategories[i].name != NULL; i++)
        printf("%s\n", kListCategories[i].name);
}

const ListCategory* getListCategory(const char* category)
{
    for (int i = 0; kListCategories[i].name != NULL; i++)
        if (strcmp(kListCategories[i].name, category) == 0)
            return &kListCategories[i];

    return NULL;
}

const ListCategory* getListCategorySafely(const char* category)
{
    const ListCategory* categoryPtr = getListCategory(category);
    ASSERT_FMT(categoryPtr != NULL, "Category %s does not exist, then cannot get it's corresponding pointer", category);
    return categoryPtr;
}

void listCommand(const char* input, const char* usage, const int numArgs)
{
    char category[MAX_INPUT] = "";
    
    if (sscanf(input, usage, category) != numArgs)
    {
        printf("Error: did not specify a category to list\n");
        return;
    }

    const ListCategory* categoryPtr = getListCategory(category);

    if (categoryPtr == NULL)
    {
        printf("Error: category %s does not exist\n", category);
        return;
    }
    
    categoryPtr->printValues();
}

const Command kCmdList[] = 
{
    {
        .name = "move",
        .shortName = "mv",
        .run = moveCommand,
        .helpText = "Moves the pieces using x and y",
        .usage = "%d %d %d %d",
        .argNames = "pos-x pos-y mov-x mov-y",
        .numArgs = 4,
    },

    {
        .name = "help",
        .shortName = "help",
        .run = helpCommand,
        .helpText = "Displays text about the command",
        .usage = "%s",
        .argNames = "cmd-name",
        .numArgs = 1,
    },

    {
        .name = "list",
        .shortName = "ls",
        .run = listCommand,
        .helpText = "Print a list of related topic",
        .usage = "%s",
        .argNames = "category",
        .numArgs = 1,
    },

    {
        .name = "set",
        .shortName = "set",
        .run = setCommand,
        .helpText = "This command allows you to set any of the available settings that exist for this program",
        .usage = "%s %s",
        .argNames = "setting new-value",
        .numArgs = 2,
    },

    { // this is going to be like a null terminator
        .name = NULL,
    },

};

// This is a placeholder funtion for incomplete commands
void fooCmdFunc(void)
{
    printf("This is a placeholder function!\n");
}

static bool gProgramIsRunning = true;
void endProgram(void)
{
    gProgramIsRunning = false;
}

void printCurrChessGame(void)
{
    printBoard(gCurrChessGame.board);
}

const NoArgsCommand kNoArgsCmdList[] =
{
    {
        .name = "print",
        .shortName = "pr",
        .run = printCurrChessGame,
        .helpText = "Prints the current state of the board to the console",
    },

    {
        .name = "restart",
        .shortName = "rs",
        .run = resetCurrChessGame,
        .helpText = "Resets the board to its initial state",
    },

    {
        .name = "quit",
        .shortName = "qt",
        .run = endProgram,
        .helpText = "Quits the program/game",
    },

    {
        .name = "clear",
        .shortName = "clr",
        .run = clearOutput,
        .helpText = "Clears the output from the console",
    },

    { // this is going to be like a null terminator
        .name = NULL,
    },

};

const NoArgsCommand* findNoArgsCmdPtr(const char* cmd)
{
    for (int i = 0; kNoArgsCmdList[i].name != NULL; i++)
        if (strcmp(cmd, kNoArgsCmdList[i].name) == 0 ||
            strcmp(cmd, kNoArgsCmdList[i].shortName) == 0)
            return &kNoArgsCmdList[i];
    
    return NULL;
}

const Command* findCmdPtr(const char* cmd)
{
    for (int i = 0; kCmdList[i].name != NULL; i++)
        if (strcmp(cmd, kCmdList[i].name) == 0 ||
            strcmp(cmd, kCmdList[i].shortName) == 0)
            return &kCmdList[i];

    return NULL;
}

void runCmdStr(const char* cmd, const char* input)
{
    const Command* cmdPtr = findCmdPtr(cmd);
    if (cmdPtr != NULL)
    {
        cmdPtr->run(input,cmdPtr->usage,cmdPtr->numArgs);
        return;
    }

    const NoArgsCommand* noArgsCmdPtr = findNoArgsCmdPtr(cmd);
    if (noArgsCmdPtr != NULL) 
    {
        noArgsCmdPtr->run();
        return;
    }

    printf("Error: Command %s does not exist!\n", cmd);
}

bool allCharsAreWS(const char* str)
{
    for (int i=0; i<(int)strlen(str); i++)
        if (!isspace(str[i]))
            return false;

    return true;
}

void commandLoop(void)
{
    clearOutput();

    char inputLine[MAX_LINE_LENGTH] = "";

    printf("Welcome to minimal-chess!\n"
           "Use \"list\" to list all commands\n"
           "And use \"help [command]\" for text about the command\n");

    resetCurrChessGame();
    printCurrChessGame();

    char cmdStr[MAX_CMD_LENGTH] = "";

    while (gProgramIsRunning)
    {
        printf("$ ");

        getLine(inputLine);

        if (allCharsAreWS(inputLine)) continue;

        sscanf(inputLine,"%s",cmdStr);

        const char* cmdArgs = strchr(inputLine,' ') == NULL ?
                              "" : strchr(inputLine,' ');

        runCmdStr(cmdStr,cmdArgs);
    }
}
