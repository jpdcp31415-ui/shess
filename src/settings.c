#include <stddef.h> // for NULL
#include <string.h>

#include "../include/settings.h"

Setting gAllSettings[] = (Setting[])
{
    /* ---OUTPUT SETTINGS--- */
    {
        .settingName = "board-theme",
        .typeOfData = STR_SETT_TYPE,
        .currData = {.strData = "emoji"},
        .defaultData = {.strData = "emoji"},
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
    const char** fmtArr = (const char*[]){"","%d","%d","%s","%c"};
    return fmtArr[t];
}

USettingType getSettingData(const char* setting)
{
    for (int i = 0; gAllSettings[i].settingName != NULL; i++)
        if (strcmp(gAllSettings[i].settingName,setting) == 0)
            return gAllSettings[i].currData;

    return (USettingType){};
}

USettingType setSettingData(const char* setting);

