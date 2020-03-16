//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_CONDITION_H
#define INCLUDED_PR_CONDITION_H

#ifndef INCLUDED_TIME_H
#define INCLUDED_TIME_H
#include <time.h>
#endif //INCLUDED_TIME_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_condition_s pr_condition_t;
typedef struct pr_mutex_s pr_mutex_t;

////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new(pr_mutex_t* mutex);

void pr_condition_delete(pr_condition_t**);

void pr_condition_wait(pr_condition_t*);

void pr_condition_waitfor(pr_condition_t*, const time_t seconds);

void pr_condition_notify(pr_condition_t*);

void pr_condition_notifyall(pr_condition_t*);

#endif //INCLUDED_PR_CONDITION_H
