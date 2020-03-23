//
// Created by PCHEN on 2020-03-20.
//

#ifndef INCLUDED_LOGGER_BUFFERVECTOR_H
#define INCLUDED_LOGGER_BUFFERVECTOR_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H


////////////////////////////////////////////////////////////////////////////////
typedef struct logger_buffervector_s logger_buffervector_t;
typedef struct logger_buffervectoriter_s logger_buffervectoriter_t;
typedef struct logger_buffer_s logger_buffer_t;

////////////////////////////////////////////////////////////////////////////////
logger_buffervector_t* logger_buffervector_new();

void logger_buffervector_delete(logger_buffervector_t**, void(*bufferReleaseFn)(logger_buffer_t*, void* args), void* args);

void logger_buffervector_add(logger_buffervector_t*, logger_buffer_t* buffer);

pr_bool_t logger_buffervector_isempty(logger_buffervector_t*);

void logger_buffervector_pop(logger_buffervector_t*, logger_buffer_t** returnBuffer);

void logger_buffervector_clear(logger_buffervector_t*, void(*releaseFn)(logger_buffer_t*, void* args), void* args);

// -----------------------------------------------------------------------------
logger_buffervectoriter_t* logger_buffervectoriter_new(logger_buffervector_t*);

void logger_buffervectoriter_delete(logger_buffervectoriter_t** iter);

pr_bool_t logger_buffervectoriter_hasmore(logger_buffervectoriter_t*);

void logger_buffervectoriter_next(logger_buffervectoriter_t*);

logger_buffer_t* logger_buffervectoriter_get(logger_buffervectoriter_t*);

void logger_buffervectoriter_remove(logger_buffervectoriter_t*);

////////////////////////////////////////////////////////////////////////////////
#define LOGGER_BUFFERVECTOR_SWAP(v1, v2) \
do{\
    void* tmp = (v1);\
    (v1)=(v2);\
    (v2)=tmp;\
}while(0)

#endif //INCLUDED_LOGGER_BUFFERVECTOR_H
