//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDED_PR_POOL_H
#define INCLUDED_PR_POOL_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_pool_s pr_pool_t;

////////////////////////////////////////////////////////////////////////////////
pr_pool_t * pr_pool_new(int objectSize, int chunkSize);

void pr_pool_delete(pr_pool_t**);

void* pr_pool_alloc(pr_pool_t*);
    // Allocate block of memory of specified number of bytes

void pr_pool_free(pr_pool_t*, void* ptr);

void pr_pool_dryup(pr_pool_t*);

#endif //INCLUDED_PR_POOL_H
