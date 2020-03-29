//
// Created by PCHEN on 2020-03-23.
//

#ifndef INCLUDED_PR_THREADPOOL_H
#define INCLUDED_PR_THREADPOOL_H
////////////////////////////////////////////////////////////////////////////////
typedef void* (*pr_threadpool_proc)(void*);

typedef struct pr_threadpool_s pr_threadpool_t;

////////////////////////////////////////////////////////////////////////////////
pr_threadpool_t* pr_threadpool_new(int numOfThreads);

void pr_threadpool_delete(pr_threadpool_t**);

void pr_threadpool_submit(pr_threadpool_t*, pr_threadpool_proc, void* args, pr_threadpool_proc callback);

void pr_threadpool_run(pr_threadpool_t*);

#endif //INCLUDED_PR_THREADPOOL_H
