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
        .settingName = "blank-fill-char",
        .typeOfData = CHAR_SETT_TYPE,
        .currData = {.charData = ' '},
        .defaultData = {.charData = ' '},
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
    const char** fmtArr = (const char*[]){"","%d","%d","%s","%c"};
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

/* USettingType setSettingData(const char* setting) */
/* { */
/*     switch (getSettingData(setting)) */
/*     { */
/*     case NULL_SETT_TYPE: */
/*     case BOOL_SETT_TYPE: */
/*     case INT_SETT_TYPE: */
/*     case STR_SETT_TYPE: */
/*     case CHAR_SETT_TYPE: */
/*     } */
/* } */
/*  */
