#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/settings.h"
#include "../include/board-theme.h"
#include "../include/assert-toggle.h"

bool isValidBoardTheme(const char* boardTheme)
{
    return getKBoardThemePtr(boardTheme) != NULL;
}

TermFormat strToFormat(const char* sett);

bool isValidColourStr(const char* col)
{
    return strToFormat(col).backCol != NULL_TERM_COLOUR &&
           strToFormat(col).foreCol != NULL_TERM_COLOUR &&
           strToFormat(col).style != INVALID_TERM_STYLE;
}

Setting gAllSettings[] =
{
    /* ---OUTPUT SETTINGS--- */
    {
        .settingName = "board-theme",
        .typeOfData = STR_SETT_TYPE,
        .currData = {.strData = "emoji"},
        .defaultData = {.strData = "emoji"},
        .isValidData = isValidBoardTheme,
    },

    {
        .settingName = "board-colour",
        .typeOfData = COLOUR_SETT_TYPE,
        .currData = {
            .colourData = {
                .backCol = BLACK_COL,
                .foreCol = WHITE_COL,
                .style = NULL_TERM_STYLE,
            }
        },
        .defaultData = {
            .colourData = {
                .backCol = BLACK_COL,
                .foreCol = WHITE_COL,
                .style = NULL_TERM_STYLE,
            }
        },
        .isValidData = isValidColourStr,
    },

    {
        .settingName = "blank-fill-char",
        .typeOfData = CHAR_SETT_TYPE,
        .currData = {.charData = '_'},
        .defaultData = {.charData = '_'},
    },

    {
        .settingName = "white-fill-char",
        .typeOfData = CHAR_SETT_TYPE,
        .currData = {.charData = '-'},
        .defaultData = {.charData = '-'},
    },

    {
        .settingName = "black-fill-char",
        .typeOfData = CHAR_SETT_TYPE,
        .currData = {.charData = '#'},
        .defaultData = {.charData = '#'},
    },

    {
        .settingName = "space-between-columns",
        .typeOfData = BOOL_SETT_TYPE,
        .currData = {.boolData = 1},
        .defaultData = {.boolData = 1},
    },

    {
        .settingName = "show-board-coords",
        .typeOfData = BOOL_SETT_TYPE,
        .currData = {.boolData = 1},
        .defaultData = {.boolData = 1},
    },

    {
        .settingName = NULL,
    },

    /* ---GAME SETTINGS--- */
};

const char* getSettingTypeFmt(const TypeOfSetting t)
{
    ASSERT(t != NULL_SETT_TYPE, "Cannot get type formar for NULL_SETT_TYPE");

    const char** fmtArr = (const char*[]){"%s", /* for "true" / "false" */
                                          "%d","%s","%c","%s"};
    return fmtArr[t - 1];
}

Setting* getSettingStruct(const char* setting)
{
    for (int i = 0; gAllSettings[i].settingName != NULL; i++)
        if (strcmp(gAllSettings[i].settingName,setting) == 0)
            return &gAllSettings[i];

    return NULL;
}

Setting* getSettingStructSafely(const char* setting)
{
    Setting* settingStruct = getSettingStruct(setting);
    ASSERT_FMT(settingStruct != NULL, "Setting %s does not exist, then cannot get it's corresponding struct", setting);
    return settingStruct;
}

void printSettings(void)
{
    for (int i = 0; gAllSettings[i].settingName != NULL; i++)
        printf("%s\n", gAllSettings[i].settingName);
}

USettingType getSettingData(const char* setting)
{
    return getSettingStruct(setting)->currData;
}

USettingType getSettingDataSafely(const char* setting)
{
    return getSettingStructSafely(setting)->currData;
}

typedef enum
{
    NO_VALUE_ERR,
    VALUE_INPUT_ERR,
    INVALID_VALUE_ERR,
} SettingValueErr;

SettingValueErr setSettingData(const char* setting, const char* strValue)
{
    Setting* setStruct = getSettingStructSafely(setting);

    const char* fmt = getSettingTypeFmt(setStruct->typeOfData);

    USettingType* data = &setStruct->currData;

    if (setStruct->isValidData != NULL && !setStruct->isValidData(strValue))
        return INVALID_VALUE_ERR;

    switch (setStruct->typeOfData)
    {
    case INT_SETT_TYPE:
        int i = 0;
        if (sscanf(strValue, fmt, &i) == 0)
            return VALUE_INPUT_ERR;
        data->intData = i;
        break;
    case BOOL_SETT_TYPE:
        char boolStr[MAX_STR_SET_DATA] = "";

        if (sscanf(strValue, fmt, boolStr) == 0)
            return VALUE_INPUT_ERR;

        if (!(strcmp(boolStr, "true") == 0 || strcmp(boolStr, "false") == 0))
            return VALUE_INPUT_ERR;

        data->boolData = strcmp(boolStr, "true") == 0 ? true : false;
        break;
    case STR_SETT_TYPE:
        char str[MAX_STR_SET_DATA] = "";
        if (sscanf(strValue, fmt, str) == 0)
            return VALUE_INPUT_ERR;
        strncpy(data->strData, str, MAX_STR_SET_DATA);
        break;
    case CHAR_SETT_TYPE:
        char c = '\0';
        if (sscanf(strValue, fmt, c) == 0)
            return VALUE_INPUT_ERR;
        data->charData = c;
        break;
    case COLOUR_SETT_TYPE:
        char colStr[MAX_STR_SET_DATA] = "";
        if (sscanf(strValue, fmt, colStr) == 0)
            return VALUE_INPUT_ERR;
        data->colourData = strToFormat(colStr);
        break;
    case NULL_SETT_TYPE:
        EXIT_MSG("This setting has a type of data NULL_SETT_TYPE which cannot be assigned");
    }

    return NO_VALUE_ERR;
}

TermColour strToColour(const char* str)
{
    if (strcmp(str, "white") == 0)      return WHITE_COL;
    if (strcmp(str, "black") == 0)      return BLACK_COL;
    if (strcmp(str, "red")   == 0)      return RED_COL;
    if (strcmp(str, "green") == 0)      return GREEN_COL;
    if (strcmp(str, "yellow") == 0)     return YELLOW_COL;
    if (strcmp(str, "blue")  == 0)      return BLUE_COL;
    if (strcmp(str, "purple") == 0)     return PURPLE_COL;
    if (strcmp(str, "cyan") == 0)       return CYAN_COL;
    if (strcmp(str, "light-gray") == 0) return LIGHT_GRAY_COL;

    return NULL_TERM_COLOUR;
}

TermStyle strToStyle(const char* str)
{
    if (strcmp(str, "normal") == 0)     return NULL_TERM_STYLE;
    if (strcmp(str, "bold") == 0)       return BOLD_FMT;
    if (strcmp(str, "faded")   == 0)    return FADED_FMT;
    if (strcmp(str, "italic") == 0)     return ITALIC_FMT;
    if (strcmp(str, "underline") == 0 ||
        strcmp(str, "under") == 0)      return UNDERLINE_FMT;
    if (strcmp(str, "blink") == 0)      return BLINK_FMT;

    return INVALID_TERM_STYLE;
}

TermColour getForeFromStr(const char* str)
{
    if (strcmp(str, "") == 0) return WHITE_COL;

    if (strchr(str, ';') == NULL) return strToColour(str);

    char fore[64] = "";
    strncpy(fore, str, strcspn(str, ";"));
    return strToColour(fore);
}

TermColour getBackFromStr(const char* str)
{
    if (strcmp(str, "") == 0) return BLACK_COL;

    const char* strPtr = strchr(str, ';') + 1;
    if (strPtr == NULL + 1) return BLACK_COL;

    char back[64] = "";
    strncpy(back, strPtr, strcspn(strPtr, ";"));
    return strToColour(back);
}

TermStyle getStyleFromStr(const char* str)
{
    if (strcmp(str, "") == 0) return NULL_TERM_STYLE;

    const char* strPtr = strchr(str, ';') + 1;
    if (strPtr == NULL + 1) return NULL_TERM_STYLE;
    strPtr = strchr(strPtr, ';') + 1;
    if (strPtr == NULL + 1) return NULL_TERM_STYLE;

    return strToStyle(strPtr);
}

TermFormat strToFormat(const char* sett)
{
    return (TermFormat) {
        .foreCol = getForeFromStr(sett),
        .backCol = getBackFromStr(sett),
        .style   = getStyleFromStr(sett),
    };
}

void setCommand(const char* input, const char* usage, const int numArgs)
{
    char setting[256] = "";
    char settingValue[256] = "";

    if (sscanf(input, usage, setting, settingValue) != numArgs)
    {
        printWMsgType(ERROR_MSG,"Input did not go as expected OR setting or value was/were not specified!");
        return;
    }

    if (getSettingStruct(setting) == NULL)
    {
        printWMsgType(ERROR_MSG,"setting \"%s\" does not exist", setting);
        return;
    }

    if (getSettingStruct(setting)->typeOfData == COLOUR_SETT_TYPE)
    {
        const TermFormat fmt = strToFormat(settingValue);

        if (fmt.foreCol == NULL_TERM_COLOUR)
            printWMsgType(ERROR_MSG,"could not get foreground colour");
        
        if (fmt.backCol == NULL_TERM_COLOUR)
            printWMsgType(ERROR_MSG,"could not get background colour");
        
        if (fmt.style == INVALID_TERM_STYLE)
            printWMsgType(ERROR_MSG,"could not get style\n");
    }

    switch (setSettingData(setting, settingValue))
    {
    case NO_VALUE_ERR: return;
    case VALUE_INPUT_ERR:
        printWMsgType(ERROR_MSG, "Input did not go well when getting new value (%s) for setting (%s)\n", settingValue, setting);
        return;
    case INVALID_VALUE_ERR:
        printWMsgType(ERROR_MSG, "Invalid value \"%s\" for setting \"%s\"\n", settingValue, setting);
        return;
    default: EXIT_MSG("Invalid value for SettingValueErr type");
    }
}
