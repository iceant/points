//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDED_PR_THREAD_ATTR_H
#define INCLUDED_PR_THREAD_ATTR_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_attr_s pr_thread_attr_t;

////////////////////////////////////////////////////////////////////////////////
pr_thread_attr_t* pr_thread_attr_new();

void pr_thread_attr_delete(pr_thread_attr_t**);

void pr_thread_attr_stacksize_set(pr_thread_attr_t*, int stacksize);
int pr_thread_attr_stacksize_get(pr_thread_attr_t*);

void pr_thread_attr_detach_set(pr_thread_attr_t*, pr_bool_t);
pr_bool_t pr_thread_attr_detach_get(pr_thread_attr_t*);

void pr_thread_attr_stacksizeparamisreservation_set(pr_thread_attr_t*, pr_bool_t);
pr_bool_t pr_thread_attr_stacksizeparamisreservation_get(pr_thread_attr_t*);

#endif //INCLUDED_PR_THREAD_ATTR_H
