/**
 * @file log.h
 * @author hongzhijun (eehongzhijun@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

#include "stdio.h"

#define HEX_TRACE_COLOR

#ifdef HEX_TRACE_COLOR
#define LOG_COLOR_BLACK "\033[40;30m"
#define LOG_COLOR_RED "\033[40;31m"
#define LOG_COLOR_GREEN "\033[40;32m"
#define LOG_COLOR_YELLOW "\033[40;33m"
#define LOG_COLOR_DARK_BLUE "\033[40;34m"
#define LOG_COLOR_PURPLE "\033[40;35m"
#define LOG_COLOR_BLUE "\033[40;36m"
#define LOG_COLOR_RESET "\033[0m"
#else
#define LOG_COLOR_BLACK
#define LOG_COLOR_RED
#define LOG_COLOR_GREEN
#define LOG_COLOR_YELLOW
#define LOG_COLOR_DARK_BLUE
#define LOG_COLOR_PURPLE
#define LOG_COLOR_BLUE
#define LOG_COLOR_RESET
#endif

/* Define trace levels */
typedef enum {
    TRACE_LEVEL_BEGIN = 0,
    TRACE_LEVEL_NONE = TRACE_LEVEL_BEGIN,
    TRACE_LEVEL_ERROR,
    TRACE_LEVEL_WARNING,
    TRACE_LEVEL_INFO,
    TRACE_LEVEL_DEBUG,
    TRACE_LEVEL_END = TRACE_LEVEL_DEBUG,
} TRACE_LEVEL;

#define LOG_DEBUG(fmt, ...)                                     \
    {                                                           \
        printf(LOG_COLOR_BLUE "[DEBUG  ] " LOG_COLOR_RESET fmt, \
               ##__VA_ARGS__);                                  \
        printf("\n");                                           \
    }

#define LOG_INFO(fmt, ...)                                       \
    {                                                            \
        printf(LOG_COLOR_GREEN "[INFO   ] " LOG_COLOR_RESET fmt, \
               ##__VA_ARGS__);                                   \
        printf("\n");                                            \
    }

#define LOG_WARNING(fmt, ...)                                     \
    {                                                             \
        printf(LOG_COLOR_YELLOW "[WARNING] " LOG_COLOR_RESET fmt, \
               ##__VA_ARGS__);                                    \
        printf("\n");                                             \
    }

#define LOG_ERROR(fmt, ...)                                                    \
    {                                                                          \
        printf(LOG_COLOR_RED "[ERROR  ] " LOG_COLOR_RESET fmt, ##__VA_ARGS__); \
        printf("\n");                                                          \
    }

#endif
