#if __STDC_VERSION__ < 199901L
    #error I need at least C99 to compile
#endif

#include <string.h>
#include "piece-traits.h"
#include "io-plus.h"

typedef Piece ChessGrid[8][8];

const ChessGrid startChessGrid = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN}, // / 1st
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}}, // \ row
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},      // / 2nd
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},     // \ row
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},      // / 8th
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},     // \ row
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN}, // / 7th
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}}, // \ row
};

ChessGrid currBoard = {
    {{BLACK,ROOK},{BLACK,KNIGHT},{BLACK,BISHOP},{BLACK,QUEEN}, // / 1st
     {BLACK,KING},{BLACK,BISHOP},{BLACK,KNIGHT},{BLACK,ROOK}}, // \ row
    {{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},      // / 2nd
     {BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN},{BLACK,PAWN}},     // \ row
[6]={{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},      // / 8th
     {WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN},{WHITE,PAWN}},     // \ row
    {{WHITE,ROOK},{WHITE,KNIGHT},{WHITE,BISHOP},{WHITE,QUEEN}, // / 7th
     {WHITE,KING},{WHITE,BISHOP},{WHITE,KNIGHT},{WHITE,ROOK}}, // \ row
};

bool isInBoardBounds(int x, int y)
{
    return ((x >= 0 && x < 8) &&
            (y >= 0 && y < 8));
}

bool isVecInBoardBounds(const IntVec2D* vec)
{
    return ((vec->x >= 0 && vec->x < 8) &&
            (vec->y >= 0 && vec->y < 8));
}

Piece* getPieceAt(ChessGrid grid, const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(grid[y][x]);
}

Piece* getCurrPieceAt(const int x, const int y)
{
    assert(isInBoardBounds(x,y) && "Getting piece that is out of bounds!");
    return &(currBoard[y][x]);
}

Piece* getPieceAtVec(ChessGrid grid, const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(grid[vec->y][vec->x]);
}

Piece* getCurrPieceAtVec(const IntVec2D* vec)
{
    assert(isInBoardBounds(vec->x,vec->y) && "Getting piece that is out of bounds!");
    return &(currBoard[vec->y][vec->x]);
}

typedef enum
{
    ONE_CHAR,
    TWO_CHARS,
    EMOJI,
} PrintMode;

PrintMode g_setPrintMode = EMOJI;

void printPiece(const Piece* p)
{
    switch (g_setPrintMode)
    {
    case ONE_CHAR:  printf("%c", getPiece1Ch(p));   break;
    case TWO_CHARS: printf("%s", getPiece2Ch(p));   break;
    case EMOJI:     printf("%s", getPieceEmoji(p)); break;
    }
}

bool g_setSpaceBetween = true;

void printBoard(ChessGrid grid)
{
    for (int y=0; y<8; y++)
    {
        for (int x=0; x<8; x++)
        {
            printPiece(getPieceAt(grid,x,y));
            if (g_setSpaceBetween)
                printf(" ");
        }
        printf("\n");
    }
}

void printCurrBoard(void)
{
    printBoard(currBoard);
}

void flipBoard(ChessGrid grid)
{
    for (int y=0; y<4; y++)
        for (int x=0; x<8; x++)
        {
            const Piece elmntCp = *getPieceAt(grid,x,y);
            *getPieceAt(grid,x,y) = *getPieceAt(grid,x,7-y);
            *getPieceAt(grid,x,7-y) = elmntCp;
        }
}

void flipCurrBoard(void)
{
    flipBoard(currBoard);
}

void setBoard(const ChessGrid fromGrid, ChessGrid toGrid)
{
    memcpy(toGrid, fromGrid, sizeof(ChessGrid));
}

void resetBoard(ChessGrid grid)
{
    memcpy(&grid, startChessGrid, sizeof(ChessGrid));
}

void resetCurrBoard(void)
{
    resetBoard(currBoard);
}

bool isRunning = true;

void endProgram(void)
{
    isRunning = false;
}

void runSetup(void)
{
    printf("Starting Setup:\n"); 
    
    // Print mode
    printf("Do you want to use Emojis or ASCII (one or two chars) for the pieces? (E/1/2)");
getAgain:
    switch (getSingleChar())
    {
    case '1': g_setPrintMode = ONE_CHAR;  break;
    case '2': g_setPrintMode = TWO_CHARS; break; // this is the classic way
    case 'E': g_setPrintMode = EMOJI;     break;
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

    printf("Do you want spaces between the pieces? (Y/n): ");
    g_setSpaceBetween = getYesOrNo();
    
    // Invert colours of emojis
    if (g_setPrintMode == EMOJI)
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
    const IntVec2D position = {getPosNumber(),7-getPosNumber()};
    const IntVec2D move     = {getPosNumber(),-getPosNumber()};

    const IntVec2D nextPostion = addVecs(&position, &move);

    if (position.x == -1 || position.y == -1 ||
        move.x == -1 || move.y == 1)
    {
        printf("Did not move because of failed input\n");
        return;
    }
    else if (!isVecInBoardBounds(&position))
    {
        printf("Error: position specified is out of bounds\n");
        return;
    }
    else if (!isVecInBoardBounds(&nextPostion))
    {
        printf("Error: position + move specified is out of bounds\n");
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
//  const char* const useText; Maybe?
} Command;

void foo(void)
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

    while (isRunning)
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
