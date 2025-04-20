/**
 * @file macro.h
 * @brief Defines utility macros.
 */

#pragma once

#ifdef FDEBUG
/**
 * @def DEBUG(feat, ...)
 * @brief Conditional debug macro.
 *
 * This macro conditionally prints debug information to the standard error
 * stream. The information printed includes the source file name, line number,
 * and the formatted arguments provided to the macro.
 *
 * @param feat Defines which debug messages will be shown (0 for all messages)
 * @param ... Variable number of arguments to be formatted and printed.
 *
 * @note The behavior of this macro depends on the existence of a preprocessor
 * symbol named `FDEBUG`. If `FDEBUG` is defined, the messages whose `feat`
 * parameter matches the value of the `FDEBUG` macro will be shown. The `FDEBUG`
 * macro is defined in the Makefile, depends on the value of the `DEBUG`
 * parameter given when calling make. Otherwise, the macro does nothing.
 *
 * @cond INTERNAL
 * FDEBUG is expected to be defined at compile time.
 * @endcond
 *
 * @example
 *
 * If `FDEBUG` is defined by compiling with `make DEBUG=PIPE`:
 *
 * ```c
 * DEBUG(PIPE, "Entering function %s\n", __func__);
 * ```
 *
 * would print the following to the standard error stream:
 *
 * ```
 * macro.h:13 -> Entering function my_function
 * ```
 *
 * But this message would not be printed due to its `feat` parameter not
 * matching the FDEBUG parameter :
 *
 * ```c
 * DEBUG(JOBS, "Calling job %d", 3)
 * ```
 */
#include <stdio.h>
#define DEBUG(feat, ...)                                                  \
    do                                                                    \
    {                                                                     \
        if (!FDEBUG || !feat || feat == FDEBUG)                           \
        {                                                                 \
            fprintf(stderr, "\x1b[34m%s:%d -> ", __FILENAME__, __LINE__); \
            fprintf(stderr, __VA_ARGS__);                                 \
            fprintf(stderr, "\x1b[0m\n");                                 \
            fflush(stderr);                                               \
        }                                                                 \
    } while (0)
#else
#define DEBUG(...) \
    do             \
    {              \
    } while (0)
#endif   // FDEBUG

#define ALWAYS  0
#define INTF_DSC 1

/**
 * @def ERROR(errcode, ...)
 * @brief Error display macro.
 *
 * This macro prints error information to the standard error
 * stream. The information printed includes the source file name, line number,
 * the formatted arguments provided to the macro and the error code.
 *
 * @param ... Variable number of arguments to be formatted and printed.
 *
 */
#define ERROR(...)                                                      \
    do                                                                  \
    {                                                                   \
        fprintf(stderr, "\x1b[1;31m%s:%d -> ", __FILENAME__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                                   \
        fprintf(stderr, "\x1b[0m\n");                                   \
        fflush(stderr);                                                 \
    } while (0)
