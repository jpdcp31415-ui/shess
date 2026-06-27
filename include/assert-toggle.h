#ifndef ASSERT_TOGGLE_H
#define ASSERT_TOGGLE_H

#define ENABLE_ASSERT 1

#if ENABLE_ASSERT
    #include <assert.h>
#else
    #define assert(X)
#endif

#endif
