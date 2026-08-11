#ifndef IO_PLUS_H
#define IO_PLUS_H

#define ENABLE_ASSERT 1

#if ENABLE_ASSERT
    #include <assert.h>
#else
    #define assert(x)
#endif

#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256
#define MAX_CMD_LENGTH 64

void getLine(char* str);

void clearInput(void);

void clearOutput(void);

typedef enum
{
    NO_ERRS,
    INPUT_ERR,
    NON_POS_ERR,
} ErrorCode;

char* getStr(void);

char getSingleChar(void);

bool getYesOrNo(void);

int getNumber(ErrorCode* const inputFailed);

int getPosNumber(ErrorCode* const inputFailed);

#endif
