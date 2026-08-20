#ifndef ASSERT_TOGGLE_H
#define ASSERT_TOGGLE_H

#include <stdio.h>
#include <stdlib.h>

#define ENABLE_ASSERTS 1

#if ENABLE_ASSERTS

    #define ASSERT(cond,msg) \
        do { \
            if (!(cond)) { \
                fprintf(stderr,"In file: %s\n" \
                               "At line: %d\n" \
                               "In function: %s\n" \
                               "Assertion: %s, failed\n" \
                               "Message: %s\n", \
                               __FILE__, \
                               __LINE__, \
                               __func__, \
                               #cond, \
                               #msg); \
                exit(1); \
            } \
        } while(0)

    #define EXIT_MSG(msg) \
        do { \
            fprintf(stderr,"In file: %s\n" \
                           "At line: %d\n" \
                           "In function: %s\n" \
                           "Message: %s\n", \
                           __FILE__, \
                           __LINE__, \
                           __func__, \
                           #msg); \
            exit(1); \
        } while(0)

    #define ASSERT_FMT(cond,fmt,...) \
        do { \
            if (!(cond)) { \
                fprintf(stderr,"In file: %s\n" \
                               "At line: %d\n" \
                               "In function: %s\n" \
                               "Assertion: %s, failed\n", \
                               __FILE__, \
                               __LINE__, \
                               __func__, \
                               #cond); \
                fprintf(stderr, "Message: " fmt "\n", __VA_ARGS__); \
                exit(1); \
            } \
        } while(0)

    #define ASSERT_EQ(v1,v2,v_fmt) \
        ASSERT_FMT((v1) == (v2), "expression is not true: " \
                                 v_fmt " == " v_fmt, v1, v2)

    #define ASSERT_NOT_EQ(v1,v2,v_fmt) \
        ASSERT_FMT((v1) != (v2), "expression is not true: " \
                                 v_fmt " != " v_fmt, v1, v2)

    #define ASSERT_GR(v1,v2,v_fmt) \
        ASSERT_FMT((v1) > (v2), "expression is not true: " \
                                 v_fmt " > " v_fmt, v1, v2)

    #define ASSERT_GR_EQ(v1,v2,v_fmt) \
        ASSERT_FMT((v1) >= (v2), "expression is not true: " \
                                 v_fmt " >= " v_fmt, v1, v2)

    #define ASSERT_LE(v1,v2,v_fmt) \
        ASSERT_FMT((v1) < (v2), "expression is not true: " \
                                 v_fmt " < " v_fmt, v1, v2)

    #define ASSERT_LE_EQ(v1,v2,v_fmt) \
        ASSERT_FMT((v1) <= (v2), "expression is not true: " \
                                 v_fmt " == " v_fmt, v1, v2)

#else

    #define ASSERT(cond,msg)
    #define ASSERT_FMT(cond,fmt,...)

    #define ASSERT_EQ(v1,v2,v_fmt)
    #define ASSERT_NOT_EQ(v1,v2,v_fmt)
    #define ASSERT_GR(v1,v2,v_fmt)
    #define ASSERT_GR_EQ(v1,v2,v_fmt)
    #define ASSERT_LE(v1,v2,v_fmt)
    #define ASSERT_LE_EQ(v1,v2,v_fmt)

#endif

#endif
