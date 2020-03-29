//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDED_PR_THREAD_H
#define INCLUDED_PR_THREAD_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef void (*pr_thread_proc)(void*);

typedef struct pr_thread_s pr_thread_t;
typedef struct pr_thread_attr_s pr_thread_attr_t;

////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t*, pr_thread_proc, void*);

void pr_thread_delete(pr_thread_t**);

pr_status_t pr_thread_join(pr_thread_t*);

pr_status_t pr_thread_detach(pr_thread_t*);

void pr_thread_sleep(unsigned long milliseconds);

void pr_thread_exit(int code);

pr_thread_id_t pr_thread_self();

int pr_thread_hardware_concurrency();

#endif //INCLUDED_PR_THREAD_H
