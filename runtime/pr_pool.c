//
// Created by PCHEN on 2020-03-16.
//

#include <pr_pool.h>
#include <pr_mem.h>
#include <pr_pool_blocklist.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct link_s{
   struct link_s * d_next_p;
};
typedef struct link_s link_t;

struct pr_pool_s{
    pr_pool_blocklist_t* d_blockAllocator_p;
    int d_objectSize;
    int d_chunkSize;
    int d_instanceCount;
    link_t * d_freeList_p;
};

////////////////////////////////////////////////////////////////////////////////
// macros
#define DEFAULT_CHUNK_SIZE 100

////////////////////////////////////////////////////////////////////////////////
// static functions
static
inline
void pr_pool_replenish(pr_pool_t* pool){
    int size = pool->d_chunkSize * pool->d_objectSize;
    char* start = (char*)pr_pool_blocklist_alloc(pool->d_blockAllocator_p, size);
    assert(start);
    char* last = &start[(pool->d_chunkSize-1)* pool->d_objectSize];
    for(char* p = start; p<last; p+=pool->d_objectSize){
        ((struct link_s*)p)->d_next_p = (struct link_s*)(p+pool->d_objectSize);
    }
    ((struct link_s*)last)->d_next_p = NULL;
    pool->d_freeList_p = (struct link_s*)start;
}

////////////////////////////////////////////////////////////////////////////////
// pool
pr_pool_t* pr_pool_new(int objectSize, int chunkSize){
    assert(objectSize>0);
    assert(chunkSize>0);

    pr_pool_t * p = MALLOC(sizeof(*p));
    assert(p);

    p->d_freeList_p = NULL;
    p->d_objectSize = ALIGN((objectSize>=sizeof(struct link_s))?objectSize:sizeof(struct link_s));
    p->d_chunkSize = chunkSize>0?chunkSize:DEFAULT_CHUNK_SIZE;
    p->d_blockAllocator_p = pr_pool_blocklist_new();
    assert(p->d_blockAllocator_p);
    p->d_instanceCount=0;
}

void pr_pool_delete(pr_pool_t** pPool){
    assert(pPool);
    assert(*pPool);
    pr_pool_t * pool = *pPool;
    if(0==pool->d_instanceCount){
        pr_pool_blocklist_delete(&pool->d_blockAllocator_p);
    }
    assert(0==pool->d_instanceCount);
}

void pr_pool_dryup(pr_pool_t* pool){
    assert(pool);
    pr_pool_blocklist_release(pool->d_blockAllocator_p);
    pool->d_freeList_p = NULL;
}

void* pr_pool_alloc(pr_pool_t* pool){
    assert(pool);
    if(!pool->d_freeList_p){
        pr_pool_replenish(pool);
    }
    link_t* p = pool->d_freeList_p;
    pool->d_freeList_p = p->d_next_p;
    ++pool->d_instanceCount;
    return p;
}

void pr_pool_free(pr_pool_t* pool, void* ptr){
    assert(pool);
    if(ptr==NULL) return;
    link_t* p = (link_t*) ptr;
    p->d_next_p = pool->d_freeList_p;
    pool->d_freeList_p = p;
    --pool->d_instanceCount;
}

int pr_pool_hasmutex(pr_pool_t* pool){
    return 0;
}
