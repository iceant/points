//
// Created by PCHEN on 2020-03-16.
//

#ifndef INCLUDE_PR_POOL_H
#define INCLUDE_PR_POOL_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_pool_s pr_pool_t;

////////////////////////////////////////////////////////////////////////////////
pr_pool_t* pr_pool_new(int objectSize, int chunkSize);

void pr_pool_delete(pr_pool_t**);

void* pr_pool_alloc(pr_pool_t*);
    // Allocate block of memory of specified number of bytes.

void pr_pool_free(pr_pool_t*, void*);
    // Return the address to the local free pool.

void pr_pool_dryup(pr_pool_t*);

int pr_pool_hasmutex(pr_pool_t*);

#endif //POINTS_PR_POOL_H
