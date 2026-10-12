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
    LIGHT_GRAY_COL,
    BLACK_COL,
    RED_COL,
    ORANGE_COL,
    BROWN_COL,
    YELLOW_COL,
    GREEN_COL,
    DARK_GREEN_COL,
    CYAN_COL,
    BLUE_COL,
    DARK_BLUE_COL,
    PURPLE_COL,
    PINK_COL,
} TermBasicColour;

typedef struct
{
    int red;
    int blue;
    int green;
} TermRgbColour;

// change this to a struct?
// to allow multiple styles
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
    TermBasicColour backCol;
    TermBasicColour foreCol;
    TermStyle style;
} TermFormat;

const char* getTermFormatAsEsc(const TermFormat* fmt);

void printRgbColEsc(const TermRgbColour* col);

void printWTermFormat(const char* str, const TermFormat* fmt);

typedef enum
{
    WARN_MSG,
    ERROR_MSG,
    INFO_MSG,
} TermMsgType;

int printWMsgType(const TermMsgType t, const char* fmt, ...);

#endif
