#if __STDC_VERSION__ < 199901L
    #error I need at least C99 to compile
#endif

#include "../include/command.h"

int main(void)
{
    commandLoop();
}
