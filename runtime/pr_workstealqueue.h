//
// Created by PCHEN on 2020-03-23.
//

#ifndef INCLUDED_PR_WORKSTEALQUEUE_H
#define INCLUDED_PR_WORKSTEALQUEUE_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H


////////////////////////////////////////////////////////////////////////////////
typedef struct pr_workstealqueue_s pr_workstealqueue_t;

////////////////////////////////////////////////////////////////////////////////
pr_workstealqueue_t* pr_worksteamqueue_new();

void pr_workstealqueue_delete(pr_workstealqueue_t**, void(*releaseFn)(void*, void*), void* args);

void pr_workstealqueue_push(pr_workstealqueue_t*, void* data);

pr_bool_t pr_workstealqueue_isempty(pr_workstealqueue_t*);

pr_bool_t pr_workstealqueue_trypop(pr_workstealqueue_t*, void** ret);

pr_bool_t pr_workstealqueue_trysteal(pr_workstealqueue_t*, void** ret);

#endif //INCLUDED_PR_WORKSTEALQUEUE_H
