#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../include/colour-escapes.h"
#include "../include/assert-toggle.h"

void appendFore3bitColAsEsc(char* str, const Term3BitColour col)
{
    ASSERT(col != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    if (col != WHITE_COL)
    {
        char foreStr[64] = "";
        sprintf(foreStr, "%d", 90 + col - WHITE_COL);
        strcat(str, foreStr);
    }
}

void appendBack3bitColAsEsc(char* str, const Term3BitColour col)
{
    ASSERT(col != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    if (col != BLACK_COL)
    {
        char backStr[64] = "";
        sprintf(backStr, ";%d", 100 + col - WHITE_COL);
        strcat(str, backStr);
    }
}

void appendForeRgbColAsEsc(char* str, const TermRgbColour* col)
{
    char foreStrRgb[64] = "";
    sprintf(foreStrRgb, "38;2;%d;%d;%d", col->red, col->green, col->blue);
    strcat(str, foreStrRgb);
}

void printRgbColEsc(const TermRgbColour* col)
{
    char esc[64] = "\e[";
    appendForeRgbColAsEsc(esc, col);
    strcat(esc, "m");
    printf("%s", esc);
}

void printWTermFormat(const char* str, const TermFormat* fmt)
{
    char esc[64] = "\e[";

    appendFore3bitColAsEsc(esc, fmt->foreCol);
    appendBack3bitColAsEsc(esc, fmt->backCol);

    if (fmt->style != NULL_TERM_STYLE)
    {
        char number[10] = "";
        sprintf(number, ";%d", fmt->style - NULL_TERM_STYLE);
        strcat(esc, number);
    }

    strcat(esc, "m");

    printf("%s%s%s", esc, str, ESC_NORMAL_FMT);
}

int printWMsgType(const TermMsgType t, const char* fmt, ...)
{
    switch (t)
    {
    case WARN_MSG:  printWTermFormat("Warning: ",
                                &(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = YELLOW_COL,
                                    .style = BOLD_FMT,
                                }); break;
    case ERROR_MSG: printWTermFormat("Error: ",
                                &(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = RED_COL,
                                    .style = BOLD_FMT,
                                }); break;
    case INFO_MSG:  printWTermFormat("Info: ",
                                &(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = BLUE_COL,
                                    .style = BOLD_FMT,
                                }); break;
    }

    va_list list;
    va_start(list, fmt);
    int n = vprintf(fmt, list);
    va_end(list);

    printf("\n");
    return n;
}
