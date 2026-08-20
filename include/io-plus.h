#ifndef IO_PLUS_H
#define IO_PLUS_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 256
#define MAX_CMD_LENGTH 64

void getLine(char* str);

void clearInput(void);

void clearOutput(void);

char getSingleChar(void);

bool getYesOrNo(void);

#endif
