//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_THREAD_H
#define INCLUDED_PR_THREAD_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_s pr_thread_t;
typedef struct pr_thread_attr_s pr_thread_attr_t;
////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t*, void (*startAddress)(void*), void* args);

void pr_thread_delete(pr_thread_t**);

int pr_thread_suspend(pr_thread_t*);

int pr_thread_resume(pr_thread_t*);

int pr_thread_join(pr_thread_t*);

void pr_thread_detach(pr_thread_t*);

void pr_thread_sleep(unsigned long milliSeconds);

void pr_thread_exit(int code);

pr_thread_id_t pr_thread_self();

#endif //INCLUDED_PR_THREAD_H
