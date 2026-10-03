#ifndef COLOUR_ESCAPES_H 
#define COLOUR_ESCAPES_H 

#define ESC_NORMAL_FMT    "\e[0m"
#define ESC_BOLD_FMT      "\e[1m"
#define ESC_FADED_FMT     "\e[2m"
#define ESC_ITALIC_FMT    "\e[3m"
#define ESC_UNDERLINE_FMT "\e[4m"
#define ESC_BLINK_FMT     "\e[5m"

typedef enum
{
    NULL_TERM_COLOUR,
    WHITE_COL,
    RED_COL,
    GREEN_COL,
    BROWN_COL,
    BLUE_COL,
    PURPLE_COL,
    CYAN_COL,
    LIGHT_GRAY_COL,
    BLACK_COL,
} TermColour;

typedef enum
{
    INVALID_TERM_STYLE,
    NULL_TERM_STYLE,
    BOLD_FMT,
    FADED_FMT,
    ITALIC_FMT,
    UNDERLINE_FMT,
    BLINK_FMT,
} TermStyle;

typedef struct
{
    TermColour backCol;
    TermColour foreCol;
    TermStyle style;
} TermFormat;

const char* getColourAsEsc(const TermFormat* fmt);

#endif
