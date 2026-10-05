#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../include/colour-escapes.h"
#include "../include/assert-toggle.h"

const char* getColourAsEsc(const TermFormat* fmt)
{
    static char esc[64] = "\e[;";
    strcpy(esc, "\e[;");

    ASSERT(fmt->foreCol != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    if (fmt->foreCol != WHITE_COL)
    {
        char foreStr[10] = "";
        sprintf(foreStr, "%d", 30 + fmt->foreCol - WHITE_COL);
        strcat(esc, foreStr);
    }

    ASSERT(fmt->backCol != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    if (fmt->backCol != BLACK_COL)
    {
        char backStr[10] = "";
        sprintf(backStr, ";%d", 40 + fmt->backCol - WHITE_COL);
        strcat(esc, backStr);
    }

    if (fmt->style != NULL_TERM_STYLE)
    {
        char number[10] = "";
        sprintf(number, ";%d", fmt->style - 1);
        strcat(esc, number);
    }

    strcat(esc, "m");

    return esc;
}

void printWTermFormat(const char* str, const TermFormat* fmt)
{
    static char strCpy[255] = "";
    strcpy(strCpy, "");

    strcat(strCpy, getColourAsEsc(fmt));
    strcat(strCpy, str);
    strcat(strCpy, ESC_NORMAL_FMT);

    printf("%s", strCpy);
}

int printWMsgType(const TermMsgType t, const char* fmt, ...)
{
    switch (t)
    {
    case WARN_MSG:  printf("%sWarning: " ESC_NORMAL_FMT , getColourAsEsc(&(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = YELLOW_COL,
                                    .style = BOLD_FMT,
                                })); break;
    case ERROR_MSG: printf("%sError: " ESC_NORMAL_FMT , getColourAsEsc(&(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = RED_COL,
                                    .style = BOLD_FMT,
                                })); break;
    case INFO_MSG:  printf("%sInfo: " ESC_NORMAL_FMT , getColourAsEsc(&(TermFormat){
                                    .backCol = BLACK_COL,
                                    .foreCol = BLUE_COL,
                                    .style = BOLD_FMT,
                                })); break;
    }

    va_list list;
    va_start(list, fmt);
    int n = vprintf(fmt, list);
    va_end(list);

    printf("\n");
    return n;
}
