//
// Created by PCHEN on 2020-03-26.
//

#ifndef INCLUDED_PR_THREAD_QUEUE_H
#define INCLUDED_PR_THREAD_QUEUE_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_queue_s pr_thread_queue_t;

////////////////////////////////////////////////////////////////////////////////
pr_thread_queue_t* pr_thread_queue_new();

void pr_thread_queue_delete(pr_thread_queue_t**, void(*releaseFn)(void*, void* args), void* args);

void pr_thread_queue_push(pr_thread_queue_t*, void*);

void* pr_thread_queue_pop(pr_thread_queue_t*);

pr_bool_t pr_thread_queue_isempty(pr_thread_queue_t*);

void pr_thread_queue_clear(pr_thread_queue_t*, void(*releaseFn)(void*, void*), void*);

int pr_thread_queue_sizeof();

#endif //INCLUDED_PR_THREAD_QUEUE_H
