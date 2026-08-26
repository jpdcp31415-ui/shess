#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

typedef union
{
    bool boolData;
    int intData;
    char charData;
    char* strData;
} USettingType;

typedef enum
{
    NULL_SETT_TYPE,
    BOOL_SETT_TYPE,
    INT_SETT_TYPE,
    STR_SETT_TYPE,
    CHAR_SETT_TYPE
} TypeOfSetting;

typedef struct
{
    const char* settingName;
    const TypeOfSetting typeOfData;
    USettingType currData;
    const USettingType defaultData;
} Setting;

extern Setting gAllSettings[];

const char* getSettingTypeFmt(const TypeOfSetting t);

USettingType getSettingData(const char* settingName);

USettingType setSettingData(const char* settingName);

// add setting for custom space character

#endif
