/**
 * @file logger.c
 * @author Paulius Juzenas
 * @brief Module for logging messages to the console.
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* ANSI C libraries                                                          */
/*****************************************************************************/
#include <stdio.h>
#include <stdarg.h>
// #include <time.h>

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "logger.h"

/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/

/*
    The different log levels
    Log levels are used to filter out messages based on their severity.
    The log level also includes all messages with a lower severity.
*/
#define DEBUG     0
#define INFO      1
#define WARNING   2
#define ERROR     3
#define FATAL     4
#define NO_LOG    5

#define LOG_LEVEL DEBUG // The current log level

void generate_timestamp(char *time_string, size_t len) {
    // time_t time_raw;
    // time(&time_raw);

    // struct tm *time_info = localtime(&time_raw);

    // strftime(time_string, len, "%H:%M:%S", time_info);

    snprintf(time_string, len, "00:00:00");
}

/**
 * @brief Logging debug messages
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void log_debug(const char *fmt, ...) {
#if LOG_LEVEL <= DEBUG
    va_list args;
    va_start(args, fmt);

    char time_string[10];
    generate_timestamp(time_string, 10);

    printf("\033[94m○\033[39m Debug   [%s]: ", time_string);
    vprintf(fmt, args);
    printf("\r\n");

    va_end(args);
#endif
}

/**
 * @brief Logging info messages
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void log_info(const char *fmt, ...) {
#if LOG_LEVEL <= INFO
    va_list args;
    va_start(args, fmt);

    char time_string[10];
    generate_timestamp(time_string, 10);

    printf("\033[94m●\033[39m Info    [%s]: ", time_string);
    vprintf(fmt, args);
    printf("\r\n");

    va_end(args);
#endif
}

/**
 * @brief Logging warning messages
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void log_warning(const char *fmt, ...) {
#if LOG_LEVEL <= WARNING
    va_list args;
    va_start(args, fmt);

    char time_string[10];
    generate_timestamp(time_string, 10);

    printf("\033[33m▲\033[39m Warning [%s]: ", time_string);
    vprintf(fmt, args);
    printf("\r\n");

    va_end(args);
#endif
}

/**
 * @brief Logging error messages
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void log_error(const char *fmt, ...) {
#if LOG_LEVEL <= ERROR
    va_list args;
    va_start(args, fmt);

    char time_string[10];
    generate_timestamp(time_string, 10);

    printf("\033[91mx\033[39m Error   [%s]: ", time_string);
    vprintf(fmt, args);
    printf("\r\n");

    va_end(args);
#endif
}

/**
 * @brief Logging fatal messages
 *
 * @param fmt Format string
 * @param ... Format arguments
 */
void log_fatal(const char *fmt, ...) {
#if LOG_LEVEL <= FATAL
    va_list args;
    va_start(args, fmt);

    char time_string[10];
    generate_timestamp(time_string, 10);

    printf("☠️ Fatal  [%s]: ", time_string);
    vprintf(fmt, args);
    printf("\r\n");

    va_end(args);
#endif
}
