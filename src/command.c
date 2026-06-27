#include <string.h>
#include "../include/board-output.h"
#include "../include/io-plus.h"
#include "../include/move-rules.h"

static bool gProgramIsRunning = true;

void endProgram(void)
{
    gProgramIsRunning = false;
}

void helpCommand(void);

void listCommands(void);

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

    while (gProgramIsRunning)
    {
        printf("> ");
        strcpy(command,getStr());
        getAndRunCommand(command);
    }
}
