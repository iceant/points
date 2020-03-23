//
// Created by PCHEN on 2020-03-20.
//

#ifndef INCLUDED_LOGGER_BUFFER_H
#define INCLUDED_LOGGER_BUFFER_H

////////////////////////////////////////////////////////////////////////////////
typedef struct logger_buffer_s logger_buffer_t;

////////////////////////////////////////////////////////////////////////////////

logger_buffer_t* logger_buffer_new(int capacity);
    // pool is bind with a thread

void logger_buffer_delete(logger_buffer_t**);

void logger_buffer_append(logger_buffer_t*, const char* content, int length);

const char* logger_buffer_get(logger_buffer_t*);

int logger_buffer_capacity(logger_buffer_t*);
    // total chunks

int logger_buffer_used(logger_buffer_t*);
    // used chunk

int logger_buffer_avail(logger_buffer_t*);
    // available blank chunk to write

void logger_buffer_reset(logger_buffer_t*);

#endif //INCLUDED_LOGGER_BUFFER_H
