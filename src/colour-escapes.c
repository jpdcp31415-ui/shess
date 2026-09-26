#include <string.h>

#include "../include/colour-escapes.h"
#include "../include/assert-toggle.h"

typedef struct
{
    char* name;
    TermColour colour;
    const char* foreFmt;
    const char* backFmt;
} TermColourStruct;

const TermColourStruct gColStructArr[] =
{
    {
        .name = "white",
        .colour = WHITE_COL,
        .foreFmt = ESC_NORMAL_FMT,
        .backFmt = ESC_NORMAL_FMT,
    },

    {
        .name = "black",
        .colour = BLACK_COL,
        .foreFmt = BLACK_FRG_COL,
        .backFmt = BLACK_BKG_COL,
    },

    {
        .name = "red",
        .colour = RED_COL,
        .foreFmt = RED_FRG_COL,
        .backFmt = RED_BKG_COL,
    },

    {
        .name = "green",
        .colour = GREEN_COL,
        .foreFmt = GREEN_FRG_COL,
        .backFmt = GREEN_BKG_COL,
    },

    {
        .name = "brown",
        .colour = BROWN_COL,
        .foreFmt = BROWN_FRG_COL,
        .backFmt = BROWN_BKG_COL,
    },

    {
        .name = "blue",
        .colour = BLUE_COL,
        .foreFmt = BLUE_FRG_COL,
        .backFmt = BLUE_BKG_COL,
    },

    {
        .name = "purple",
        .colour = PURPLE_COL,
        .foreFmt = PURPLE_FRG_COL,
        .backFmt = PURPLE_BKG_COL,
    },

    {
        .name = "cyan",
        .colour = CYAN_COL,
        .foreFmt = CYAN_FRG_COL,
        .backFmt = CYAN_BKG_COL,
    },

    {
        .name = "light-gray",
        .colour = LIGHT_GRAY_COL,
        .foreFmt = LIGHT_GRAY_FRG_COL,
        .backFmt = LIGHT_GRAY_BKG_COL,
    },

    {
        .name = NULL,
    },
};

const TermColourStruct* getTermColStructByName(const char* name)
{
    for (int i = 0; gColStructArr[i].name != NULL; i++)
        if (strcmp(name, gColStructArr[i].name) == 0)
            return &gColStructArr[i];

    return NULL;
}

const TermColourStruct* getTermColStructByCol(const TermColour col)
{
    for (int i = 0; gColStructArr[i].name != NULL; i++)
        if (col == gColStructArr[i].colour)
            return &gColStructArr[i];

    return NULL;
}

const char* getFormatColEsc(const TermFormat fmt)
{
    switch (fmt)
    {
    case NORMAL_FMT:    return ESC_NORMAL_FMT;
    case BOLD_FMT:      return ESC_BOLD_FMT;
    case FADED_FMT:     return ESC_FADED_FMT;
    case ITALIC_FMT:    return ESC_ITALIC_FMT;
    case UNDERLINE_FMT: return ESC_UNDERLINE_FMT;
    case BLINK_FMT:     return ESC_BLINK_FMT;
    }

    EXIT_MSG("Not a valid value for TermFormat");
}

const char* getForegroundColEsc(const TermColour col)
{
    const char* foreEsc = getTermColStructByCol(col)->foreFmt;
    ASSERT(foreEsc != NULL, "Not a valid value for TermColour");
    return foreEsc;
}

const char* getBackgroundColEsc(const TermColour col)
{
    const char* backEsc = getTermColStructByCol(col)->backFmt;
    ASSERT(backEsc != NULL, "Not a valid value for TermColour");
    return backEsc;
}

const char* getAllColourEscape(const ColourFmt* colFmt)
{
    static char str[64] = "";
    strncpy(str, "", 64);

    strcat(str, getFormatColEsc(colFmt->format));

    if      (colFmt->foreOrBack == FOREGROUND_COL) strcat(str, getForegroundColEsc(colFmt->colour));
    else if (colFmt->foreOrBack == BACKGROUND_COL) strcat(str, getBackgroundColEsc(colFmt->colour));
    else EXIT_MSG("This foreOfBack value is not valid!");

    return str;
}

const char* matchColour(const char* colName, const ForeOrBackCol bOrF)
{
    const TermColour colValue = getTermColStructByName(colName)->colour;

    const char* foreEsc = getForegroundColEsc(colValue);
    const char* backEsc = getBackgroundColEsc(colValue);

    ASSERT(backEsc != NULL, "Not a valid value for TermColour");
    ASSERT(foreEsc != NULL, "Not a valid value for TermColour");

    if (bOrF == BACKGROUND_COL) return backEsc;
    if (bOrF == FOREGROUND_COL) return foreEsc;

    EXIT_MSG("This foreOfBack value is not valid!");
}
