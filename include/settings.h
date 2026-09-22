#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>

#define MAX_STR_SET_DATA 64

typedef union
{
    bool boolData;
    int intData;
    char charData;
    char strData[MAX_STR_SET_DATA];
} USettingType;

typedef enum
{
    NULL_SETT_TYPE,
    BOOL_SETT_TYPE,
    INT_SETT_TYPE,
    STR_SETT_TYPE,
    CHAR_SETT_TYPE,
} TypeOfSetting;

typedef struct
{
    const char* settingName;
    const TypeOfSetting typeOfData;
    USettingType currData;
    const USettingType defaultData;
    bool(*const isValidData)(const char*);
} Setting;

extern Setting gAllSettings[];

const char* getSettingTypeFmt(const TypeOfSetting t);

USettingType getSettingData(const char* settingName);

void setCommand(const char* input, const char* usage, const int numArgs);

#endif
