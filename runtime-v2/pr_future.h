//
// Created by PCHEN on 2020-03-28.
//

#ifndef INCLUDED_PR_FUTURE_H
#define INCLUDED_PR_FUTURE_H
////////////////////////////////////////////////////////////////////////////////
typedef void* (*pr_future_function)(void*);
typedef struct pr_future_s pr_future_t;

////////////////////////////////////////////////////////////////////////////////
pr_future_t* pr_future_new(pr_future_function, void* args);

void pr_future_delete(pr_future_t**);

void* pr_future_get(pr_future_t*);

#endif //INCLUDED_PR_FUTURE_H
