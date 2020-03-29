//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDE_PR_POOL_BLOCKLIST_H
#define INCLUDE_PR_POOL_BLOCKLIST_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_pool_blocklist_s pr_pool_blocklist_t;

////////////////////////////////////////////////////////////////////////////////
pr_pool_blocklist_t* pr_pool_blocklist_new();

void pr_pool_blocklist_delete(pr_pool_blocklist_t**);

void* pr_pool_blocklist_alloc(pr_pool_blocklist_t*, int nBytes);

void pr_pool_blocklist_release(pr_pool_blocklist_t*);

#endif //INCLUDE_PR_POOL_BLOCKLIST_H
