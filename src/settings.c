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

bool isValidRgbCol(const TermRgbColour* col)
{
    return col->red   >= 0   &&
           col->red   <= 255 &&
           col->blue  >= 0   &&
           col->blue  <= 255 &&
           col->green >= 0   &&
           col->green <= 255;
}

TermRgbColour strToRgbColour(const char* str);

bool isValidRgbColStr(const char* str)
{
    const TermRgbColour col = strToRgbColour(str);
    return isValidRgbCol(&col);
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
                .red   = 255,
                .green = 255,
                .blue  = 255,
            }
        },
        .defaultData = {
            .colourData = {
                .red   = 255,
                .green = 255,
                .blue  = 255,
            }
        },
        .isValidData = isValidRgbColStr,
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

TermRgbColour strToRgbColour(const char* str);

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
        data->colourData = strToRgbColour(colStr);
        break;
    case NULL_SETT_TYPE:
        EXIT_MSG("This setting has a type of data NULL_SETT_TYPE which cannot be assigned");
    }

    return NO_VALUE_ERR;
}

TermRgbColour getColFromStr(const char* str)
{
    TermRgbColour col = {};

    int n = sscanf(str, "%d,%d,%d", &col.red, &col.blue, &col.blue);
    if (n != 0) return (TermRgbColour){-1,-1,-1};

    return col;
}

TermRgbColour strToRgbColour(const char* str)
{
    if (strlen(str) > 7) return (TermRgbColour){-1, -1, -1};

    char redStr[8] = "";
    char greenStr[8] = "";
    char blueStr[8] = "";

    int n = sscanf(str, "#%2s%2s%2s", redStr, greenStr, blueStr);
    if (n != 3) return (TermRgbColour){-1, -1, -1};

    TermRgbColour col = {};

    const int a = sscanf(redStr, "%x", &col.red);
    const int b = sscanf(greenStr, "%x", &col.green);
    const int c = sscanf(blueStr, "%x", &col.blue);

    if (a + b + c != 3) return (TermRgbColour){-1, -1, -1};

    return col;
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
        const TermRgbColour col = strToRgbColour(settingValue);

        if (col.red == -1 || col.green == -1 || col.blue == -1)
            printWMsgType(ERROR_MSG, "could not get RGB colour");
    }

    switch (setSettingData(setting, settingValue))
    {
    case NO_VALUE_ERR: return;
    case VALUE_INPUT_ERR:
        printWMsgType(ERROR_MSG, "Input did not go well when getting new value (%s) for setting (%s)", settingValue, setting);
        return;
    case INVALID_VALUE_ERR:
        printWMsgType(ERROR_MSG, "Invalid value \"%s\" for setting \"%s\"", settingValue, setting);
        return;
    default: EXIT_MSG("Invalid value for SettingValueErr type");
    }
}
