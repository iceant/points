//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_MUTEX_H
#define INCLUDED_PR_MUTEX_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_mutex_s pr_mutex_t;

////////////////////////////////////////////////////////////////////////////////
pr_mutex_t * pr_mutex_new();

void pr_mutex_delete(pr_mutex_t**);

void pr_mutex_lock(pr_mutex_t*);

void pr_mutex_unlock(pr_mutex_t*);

void* pr_mutex_raw(pr_mutex_t*);

#endif //POINTS_PR_MUTEX_H
