#include <string.h>

#include "../include/assert-toggle.h"
#include "../include/command.h"
#include "../include/board-theme.h"
#include "../include/settings.h"

void printCommands(void)
{
    for (int i = 0; kNoArgsCmdList[i].name != NULL; i++)
    {
        const NoArgsCommand* cmd = &kNoArgsCmdList[i];
        if (strcmp(cmd->name,cmd->shortName) != 0)
            printf("%s / %s\n", cmd->name, cmd->shortName);
        else
            printf("%s\n", cmd->name);
    }

    for (int i = 0; kCmdList[i].name != NULL; i++)
    {
        const Command* cmd = &kCmdList[i];
        if (strcmp(cmd->name,cmd->shortName) != 0)
            printf("%s / %s [%s]\n", cmd->name, cmd->shortName, cmd->argNames);
        else
            printf("%s [%s]\n", cmd->name, cmd->argNames);
    }
}

typedef struct {
    const char* name;
    void(*const printValues)(void);
} ListCategory;

void printCategories(void);

const ListCategory kListCategories[] =
{
    {
        .name = "board-themes",
        .printValues = printBoardThemes,
    },

    {
        .name = "settings",
        .printValues = printSettings,
    },

    {
        .name = "commands",
        .printValues = printCommands,
    },

    {
        .name = "categories",
        .printValues = printCategories,
    },

    { // null terminator
        .name = NULL,
    },
};

void printCategories(void)
{
    for (int i = 0; kListCategories[i].name != NULL; i++)
        printf("%s\n", kListCategories[i].name);
}

const ListCategory* getListCategory(const char* category)
{
    for (int i = 0; kListCategories[i].name != NULL; i++)
        if (strcmp(kListCategories[i].name, category) == 0)
            return &kListCategories[i];

    return NULL;
}

const ListCategory* getListCategorySafely(const char* category)
{
    const ListCategory* categoryPtr = getListCategory(category);
    ASSERT_FMT(categoryPtr != NULL, "Category %s does not exist, then cannot get it's corresponding pointer", category);
    return categoryPtr;
}

void listCommand(const char* input, const char* usage, const int numArgs)
{
    char category[64] = "";
    
    if (sscanf(input, usage, category) != numArgs)
    {
        printf("Error: did not specify a category to list\n");
        return;
    }

    const ListCategory* categoryPtr = getListCategory(category);

    if (categoryPtr == NULL)
    {
        printf("Error: category \"%s\" does not exist\n", category);
        return;
    }
    
    categoryPtr->printValues();
}
