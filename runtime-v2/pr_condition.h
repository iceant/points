//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDED_PR_CONDITION_H
#define INCLUDED_PR_CONDITION_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_condition_s pr_condition_t;
typedef struct pr_mutex_s pr_mutex_t;

////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new();

void pr_condition_delete(pr_condition_t**);

void pr_condition_wait(pr_condition_t*);

void pr_condition_waitfor(pr_condition_t*, const pr_time_t seconds);

void pr_condition_notify(pr_condition_t*);

void pr_condition_notifyall(pr_condition_t*);

#endif //INCLUDED_PR_CONDITION_H
