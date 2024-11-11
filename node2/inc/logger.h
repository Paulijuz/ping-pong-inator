/**
 * @file logger.h
 * @author Paulius Juzenas
 * @brief Header file for logger.c
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
void log_debug(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_fatal(const char *fmt, ...);

// clang-format off
/**
 * @brief Logs an integer (debug) whenever it changes
 */
#define LOG_INT(x)                         \
    static int x##_debug_state_storage;    \
    if (x##_debug_state_storage != x) {    \
        x##_debug_state_storage = x;       \
        log_debug("'%s': %d", #x, x);      \
    }
// clang-format on
