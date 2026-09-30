#include <stdio.h>
#include <string.h>

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
        sprintf(foreStr, "%d", 30 + fmt->foreCol - 2);
        strcat(esc, foreStr);
    }

    ASSERT(fmt->backCol != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    if (fmt->backCol != WHITE_COL)
    {
        char backStr[10] = "";
        sprintf(backStr, ";%d", 40 + fmt->backCol - 2);
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
