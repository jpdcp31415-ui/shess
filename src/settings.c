#include <linux/limits.h>
#include <stdbool.h>
#include <stddef.h> // for NULL
#include <stdio.h>
#include <string.h>

#include "../include/settings.h"
#include "../include/board-theme.h"
#include "../include/assert-toggle.h"

bool isValidBoardTheme(const char* boardTheme)
{
    return getKBoardThemePtr(boardTheme) != NULL;
}

Setting gAllSettings[] = (Setting[])
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
    const char** fmtArr = (const char*[]){"","%s" /* for "true" / "false" */,"%d","%s","%c"};
    return fmtArr[t];
}

Setting* getSettingStruct(const char* setting)
{
    for (int i = 0; gAllSettings[i].settingName != NULL; i++)
        if (strcmp(gAllSettings[i].settingName,setting) == 0)
            return &gAllSettings[i];

    return NULL;
}

USettingType getSettingData(const char* setting)
{
    return getSettingStruct(setting)->currData;
}

typedef enum
{
    NO_VALUE_ERR,
    VALUE_INPUT_ERR,
    INVALID_VALUE_ERR,
} SettingValueErr;

SettingValueErr setSettingData(const char* setting, const char* strValue)
{
    Setting* setStruct = getSettingStruct(setting);

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
        char boolStr[MAX_INPUT] = "";

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
    case NULL_SETT_TYPE:
        EXIT_MSG("This setting has a type of data NULL_SETT_TYPE which cannot be assigned");
    }

    return NO_VALUE_ERR;
}

void setCommand(const char* input, const char* usage, const int numArgs)
{
    char setting[256] = "";
    char settingValue[256] = "";

    if (sscanf(input, usage, setting, settingValue) != numArgs)
    {
        printf("Error: Input did not go as expected OR setting or value was/were not specified!\n");
        return;
    }

    if (getSettingStruct(setting) == NULL)
    {
        printf("Error: Setting: %s does not exist\n", setting);
        return;
    }

    switch (setSettingData(setting, settingValue))
    {
    case NO_VALUE_ERR: return;
    case VALUE_INPUT_ERR:
        printf("Error: Input did not go well when getting new value (%s) for setting (%s)\n", settingValue, setting);
        return;
    case INVALID_VALUE_ERR:
        printf("Error: Invalid value \"%s\" for setting \"%s\"\n", settingValue, setting);
        return;
    default: EXIT_MSG("Invalid value for SettingValueErr type");
    }
}
