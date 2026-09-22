#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

typedef struct
{
    const char* const name;
    const char* const shortName;
    void(*const run)(const char* input, const char* usage, const int numArgs);
    const char* const helpText;
    const char* usage;
    const int numArgs;
} Command;

extern const Command kCmdList[];

typedef struct
{
    const char* const name;
    const char* const shortName;
    void(*const run)(void);
    const char* const helpText;
} NoArgsCommand;

extern const NoArgsCommand kNoArgsCmdList[];

/* Command functions */

void moveCommand(const char* input, const char* usage, const int numArgs);

void helpCommand(const char* input, const char* usage, const int numArgs);

void printCurrChessGame(void);

void listCommands(void);

void endProgram(void);

void fooCmdFunc(void); // Placeholder function for incomplete commands

/* Finding the command */

const Command* findCmdPtr(const char* cmd);

const NoArgsCommand* findNoArgsCmdPtr(const char* cmd);

int indexOfCmd(const char* const cmd);

void getAndRunCommand(const char* const cmd);

void commandLoop(void);

#endif
