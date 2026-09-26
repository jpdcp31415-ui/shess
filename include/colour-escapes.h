#ifndef COLOUR_ESCAPES_H 
#define COLOUR_ESCAPES_H 

#define BLACK_FRG_COL      "\e[30m"
#define RED_FRG_COL        "\e[31m"
#define GREEN_FRG_COL      "\e[32m"
#define BROWN_FRG_COL      "\e[33m"
#define BLUE_FRG_COL       "\e[34m"
#define PURPLE_FRG_COL     "\e[35m"
#define CYAN_FRG_COL       "\e[36m"
#define LIGHT_GRAY_FRG_COL "\e[37m"

#define BLACK_BKG_COL      "\e[40m"
#define RED_BKG_COL        "\e[41m"
#define GREEN_BKG_COL      "\e[42m"
#define BROWN_BKG_COL      "\e[43m"
#define BLUE_BKG_COL       "\e[44m"
#define PURPLE_BKG_COL     "\e[45m"
#define CYAN_BKG_COL       "\e[46m"
#define LIGHT_GRAY_BKG_COL "\e[47m"

#define ESC_NORMAL_FMT    "\e[0m"
#define ESC_BOLD_FMT      "\e[1m"
#define ESC_FADED_FMT     "\e[2m"
#define ESC_ITALIC_FMT    "\e[3m"
#define ESC_UNDERLINE_FMT "\e[4m"
#define ESC_BLINK_FMT     "\e[5m"

typedef enum
{
    WHITE_COL,
    BLACK_COL,
    RED_COL,
    GREEN_COL,
    BROWN_COL,
    BLUE_COL,
    PURPLE_COL,
    CYAN_COL,
    LIGHT_GRAY_COL,
} TermColour;

typedef enum
{
    NORMAL_FMT,
    BOLD_FMT,
    FADED_FMT,
    ITALIC_FMT,
    UNDERLINE_FMT,
    BLINK_FMT,
} TermFormat;

typedef enum
{
    BACKGROUND_COL,
    FOREGROUND_COL,
} ForeOrBackCol;

typedef struct
{
    TermColour colour;
    TermFormat format;
    ForeOrBackCol foreOrBack;
} ColourFmt;

const char* getColourEscape(const ColourFmt* colFmt);
const char* matchColour(const char* colName, const ForeOrBackCol bOrF);

#endif
