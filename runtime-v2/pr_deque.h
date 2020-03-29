//
// Created by PCHEN on 2020-03-26.
//

#ifndef INCLUDE_PR_DEQUE_H
#define INCLUDE_PR_DEQUE_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_deque_s pr_deque_t;

////////////////////////////////////////////////////////////////////////////////
pr_deque_t* pr_deque_new();

void pr_deque_delete(pr_deque_t**, void(*releaseFn)(void*, void*), void*);

void pr_deque_push_front(pr_deque_t*, void*);

void* pr_deque_pop_front(pr_deque_t*);

void pr_deque_push_back(pr_deque_t*, void*);

void* pr_deque_pop_back(pr_deque_t*);

pr_bool_t pr_deque_isempty(pr_deque_t*);

#endif //INCLUDE_PR_DEQUE_H
