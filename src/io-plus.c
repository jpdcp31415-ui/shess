#include "../include/io-plus.h"

void getLine(char* str)
{
	fgets(str, MAX_LINE_LENGTH, stdin);
}

void clearInput(void)
{
    if (getchar() != '\n')
        fgets((char[MAX_LINE_LENGTH]){""}, MAX_LINE_LENGTH, stdin);
}

void clearOutput(void)
{
	printf("\e[1;1H\e[2J");
}

char getSingleChar(void)
{
    char inputLine[MAX_LINE_LENGTH] = "";
    getLine(inputLine);
	return inputLine[0];
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
