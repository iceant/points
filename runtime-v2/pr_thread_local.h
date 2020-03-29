//
// Created by PCHEN on 2020-03-26.
//

#ifndef INCLUDED_PR_THREAD_LOCAL_H
#define INCLUDED_PR_THREAD_LOCAL_H
////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_local_s pr_thread_local_t;

////////////////////////////////////////////////////////////////////////////////
pr_thread_local_t* pr_thread_local_new(void(*destructor)(void*));

void pr_thread_local_delete(pr_thread_local_t**);

void pr_thread_local_set(pr_thread_local_t*, void*);

void* pr_thread_local_get(pr_thread_local_t*);

#endif //INCLUDED_PR_THREAD_LOCAL_H
