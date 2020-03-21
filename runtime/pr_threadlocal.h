//
// Created by PCHEN on 2020-03-20.
//

#ifndef INCLUDED_PR_THREADLOCAL_H
#define INCLUDED_PR_THREADLOCAL_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_threadlocal_s pr_threadlocal_t;

////////////////////////////////////////////////////////////////////////////////
pr_threadlocal_t* pr_threadlocal_new(void(*destructor)(void*));

void pr_threadlocal_delete(pr_threadlocal_t**);

void pr_threadlocal_set(pr_threadlocal_t*, void* data);

void* pr_threadlocal_get(pr_threadlocal_t*);

#endif //INCLUDED_PR_THREADLOCAL_H
