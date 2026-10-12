#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../include/colour-escapes.h"
#include "../include/assert-toggle.h"

void appendForeBasicColAsEsc(char* str, const TermBasicColour col)
{
    switch (col)
    {
    case WHITE_COL:      return;
    case LIGHT_GRAY_COL: strcat(str, "38;2;128;128;128"); break;
    case BLACK_COL:      strcat(str, "38;2;0;0;0");       break;
    case RED_COL:        strcat(str, "38;2;255;50;50");   break;
    case ORANGE_COL:     strcat(str, "38;2;255;128;0");   break;
    case YELLOW_COL:     strcat(str, "38;2;255;255;0");   break;
    case GREEN_COL:      strcat(str, "38;2;0;255;0");     break;
    case DARK_GREEN_COL: strcat(str, "38;2;0;128;0");     break;
    case CYAN_COL:       strcat(str, "38;2;0;255;255");   break;
    case BLUE_COL:       strcat(str, "38;2;0;128;255");   break;
    case DARK_BLUE_COL:  strcat(str, "38;2;0;0;255");     break;
    case PURPLE_COL:     strcat(str, "38;2;128;0;255");   break;
    case PINK_COL:       strcat(str, "38;2;255;0;255");   break;
    case NULL_TERM_COLOUR:
        ASSERT(col != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
    }
}

void appendBackBasicColAsEsc(char* str, const TermBasicColour col)
{
    switch (col)
    {
    case WHITE_COL:      strcat(str, "48;2;255;255;255"); break;
    case LIGHT_GRAY_COL: strcat(str, "48;2;128;128;128"); break;
    case BLACK_COL:      return;
    case RED_COL:        strcat(str, "48;2;255;50;50");   break;
    case ORANGE_COL:     strcat(str, "48;2;255;128;0");   break;
    case YELLOW_COL:     strcat(str, "48;2;255;255;0");   break;
    case GREEN_COL:      strcat(str, "48;2;0;255;0");     break;
    case DARK_GREEN_COL: strcat(str, "48;2;0;128;0");     break;
    case CYAN_COL:       strcat(str, "48;2;0;255;255");   break;
    case BLUE_COL:       strcat(str, "48;2;0;128;255");   break;
    case DARK_BLUE_COL:  strcat(str, "48;2;0;0;255");     break;
    case PURPLE_COL:     strcat(str, "48;2;128;0;255");   break;
    case PINK_COL:       strcat(str, "48;2;255;0;255");   break;
    case NULL_TERM_COLOUR:
        ASSERT(col != NULL_TERM_COLOUR, "Cannot get colour escape number from NULL_TERM_COLOUR");
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

    appendForeBasicColAsEsc(esc, fmt->foreCol);
    
    if (fmt->foreCol != WHITE_COL && fmt->backCol != BLACK_COL)
        strcat(esc, ";");
    
    appendBackBasicColAsEsc(esc, fmt->backCol);

    if (fmt->style != NULL_TERM_STYLE)
    {
        char number[10] = "";
        strcat(esc, ";");
        sprintf(number, "%d", fmt->style - NULL_TERM_STYLE);
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
