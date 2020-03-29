//
// Created by PCHEN on 2020-03-26.
//

#ifndef INCLUDED_PR_THREAD_POOL_H
#define INCLUDED_PR_THREAD_POOL_H
////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_pool_s pr_thread_pool_t;

typedef void* (*pr_thread_pool_proc)(void*);

typedef void (*pr_thread_pool_callback)(pr_thread_pool_t*, void* proc_ret);

typedef struct pr_thread_pool_s pr_thread_pool_t;

typedef void (*pr_thread_pool_stopCallback)(pr_thread_pool_t*, void*);

////////////////////////////////////////////////////////////////////////////////
pr_thread_pool_t* pr_thread_pool_new(int numOfWorkers);

void pr_thread_pool_delete(pr_thread_pool_t**);

void pr_thread_pool_submit(pr_thread_pool_t*, pr_thread_pool_proc, void*, pr_thread_pool_callback);

void pr_thread_pool_stop(pr_thread_pool_t*, pr_thread_pool_stopCallback, void* args);

#endif //INCLUDED_PR_THREAD_POOL_H
