//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_SCOPEGUARD_H
#define INCLUDED_PR_SCOPEGUARD_H

#ifndef INCLUDED_PR_TYPES_H
#include <pr_types.h>
#endif //INCLUDED_PR_TYPES_H

////////////////////////////////////////////////////////////////////////////////
typedef void (*pr_scopeguard_proc)(void*);
typedef struct pr_scopeguard_s pr_scopeguard_t;

////////////////////////////////////////////////////////////////////////////////
pr_scopeguard_t* pr_scopeguard_new();

void pr_scopeguard_delete(pr_scopeguard_t**);

void pr_scopeguard_onexit(pr_scopeguard_t*, pr_scopeguard_proc proc, void* args);

void pr_scopeguard_dismiss(pr_scopeguard_t*);

pr_bool_t pr_scopeguard_isdismissed(pr_scopeguard_t*);

#endif //INCLUDED_PR_SCOPEGUARD_H
