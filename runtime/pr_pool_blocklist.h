//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_POOL_BLOCKLIST_H
#define INCLUDED_PR_POOL_BLOCKLIST_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_pool_blocklist_s pr_pool_blocklist_t;

////////////////////////////////////////////////////////////////////////////////
pr_pool_blocklist_t* pr_pool_blocklist_new();

void pr_pool_blocklist_delete(pr_pool_blocklist_t**);

void* pr_pool_blocklist_alloc(pr_pool_blocklist_t*, int nbytes);

void pr_pool_blocklist_release(pr_pool_blocklist_t*);

#endif //POINTS_PR_POOL_BLOCKLIST_H
