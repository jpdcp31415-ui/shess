#include <stdbool.h>

void endProgram(void);

void helpCommand(void);

void listCommands(void);

typedef struct
{
    const char* const name;
    void(*getAndRun)(void);
    const char* const helpText;
} Command;

// This is a placeholder funtion for incomplete commands
void fooCmdFunc(void);

int indexOfCmd(const char* const cmd);

void getAndRunCommand(const char* const cmd);

void commandLoop(void);
