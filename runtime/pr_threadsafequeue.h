//
// Created by PCHEN on 2020-03-23.
//

#ifndef INCLUDED_PR_THREADSAFEQUEUE_H
#define INCLUDED_PR_THREADSAFEQUEUE_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H


////////////////////////////////////////////////////////////////////////////////
typedef struct pr_threadsafequeue_s pr_threadsafequeue_t;

////////////////////////////////////////////////////////////////////////////////
pr_threadsafequeue_t * pr_threadsafequeue_new();

void pr_threadsafequeue_delete(pr_threadsafequeue_t**, void(*releaseFn)(void* value, void* args), void* args);

void pr_threadsafequeue_push(pr_threadsafequeue_t*, void*);

void pr_threadsafequeue_pop(pr_threadsafequeue_t*, void**);

pr_bool_t pr_threadsafequeue_trypop(pr_threadsafequeue_t*, void**);

pr_bool_t pr_threadsafequeue_isempty(pr_threadsafequeue_t*);

#endif //INCLUDED_PR_THREADSAFEQUEUE_H
