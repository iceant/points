//
// Created by PCHEN on 2020-03-20.
//

#ifndef INCLUDED_LOGGER_H
#define INCLUDED_LOGGER_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct logger_s logger_t;
typedef enum{
    eLOGGER_LEVEL_FATAL,
    eLOGGER_LEVEL_ERROR,
    eLOGGER_LEVEL_WARN,
    eLOGGER_LEVEL_INFO,
    eLOGGER_LEVEL_DEBUG
}logger_level_t;

typedef void (*logger_stopCallback)(logger_t* logger, void* args);

////////////////////////////////////////////////////////////////////////////////
logger_t* logger_new(const char* file, logger_level_t loggerLevel);

void logger_delete(logger_t**);

void logger_printf(logger_t*, logger_level_t level, const char* file, int line, const char* fmt, ...);

void logger_start(logger_t*, void(*startCallback)(logger_t*, void* args), void* args);

void logger_stop(logger_t*, logger_stopCallback, void* args);

pr_bool_t logger_isrunning(logger_t*);

pr_bool_t logger_isstopped(logger_t*);

////////////////////////////////////////////////////////////////////////////////
#define LOGGER_FATAL(logger, fmt, ...) logger_printf(logger, eLOGGER_LEVEL_FATAL, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOGGER_ERROR(logger, fmt, ...) logger_printf(logger, eLOGGER_LEVEL_ERROR, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOGGER_WARN(logger, fmt, ...) logger_printf(logger, eLOGGER_LEVEL_WARN, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOGGER_INFO(logger, fmt, ...) logger_printf(logger, eLOGGER_LEVEL_INFO, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOGGER_DEBUG(logger, fmt, ...) logger_printf(logger, eLOGGER_LEVEL_DEBUG, __FILE__, __LINE__, fmt, __VA_ARGS__)

#endif //INCLUDED_LOGGER_H
