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
#define MAX_STR_LENGTH 64

char* getLine(void)
{
	return fgets((char[MAX_LINE_LENGTH]){""}, MAX_LINE_LENGTH, stdin);
}

void clearInput(void)
{
    if (getchar() != '\n')
        getLine();
}

char* getStr(void)
{
    static char str[MAX_STR_LENGTH] = "";
	assert(scanf("%s", str) && "Input failed when getting string");
    return str;
}

char getSingleChar(void)
{
	return getStr()[0];
}

bool getYesOrNo(void)
{
    char ch = '\0';

    while (true)
    {
        ch = getSingleChar();

        switch (ch)
        {
        case 'y':
        case 'Y':
            return true;
        case 'n':
        case 'N':
            return false;
        default: 
            printf("The character inputted is invalid.\nTry again: ");
            continue;
        }
    } 
}

void clearOutput(void)
{
	printf("\e[1;1H\e[2J");
}

#endif
