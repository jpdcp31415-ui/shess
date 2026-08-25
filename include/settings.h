#include <stdbool.h>

typedef struct
{
    const char* typeOfData;
    const char* currData;
    const char* defaultData;
    const char* settingName;
} Setting;

typedef struct
{
    const Setting* settings;
    const char* settingCategory;
} SettingGroup;

// add setting for custom space char

typedef struct
{
    const char* boardTheme;
    bool invertColours;
    bool spaceBetween;
    bool emptySpaceAsUnderscore;
    bool showPrintCoords;
} OutputSettings;

OutputSettings gOutputSettings = 
{
    .boardTheme = "emoji",
    .invertColours = true,
    .spaceBetween = true,
    .showPrintCoords = true,
};

