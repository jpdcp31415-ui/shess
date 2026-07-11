#include "../include/io-plus.h"

char* getLine(void)
{
	return fgets((char[MAX_LINE_LENGTH]){""}, MAX_LINE_LENGTH, stdin);
}

void clearInput(void)
{
    if (getchar() != '\n')
        getLine();
}

void clearOutput(void)
{
	printf("\e[1;1H\e[2J");
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

int getNumber(ErrorCode* const inputFailed)
{
    int n = 0;

    if (!scanf("%d", &n)) *inputFailed = INPUT_ERR;
    else *inputFailed = NO_ERRS;

    return n;
}

int getPosNumber(ErrorCode* const inputFailed)
{
    int n = getNumber(inputFailed);

    if (n < 0) *inputFailed = NON_POS_ERR;

    return n;
}
