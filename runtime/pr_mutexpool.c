//
// Created by PCHEN on 2020-03-17.
//
#include <pr_pool.h>
#include <pr_pool_blocklist.h>
#include <pr_mem.h>
#include <pr_mutex.h>
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
    pr_mutex_t* d_lock_p;
};

////////////////////////////////////////////////////////////////////////////////
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
    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);
    return p;
}

void pr_pool_delete(pr_pool_t** pPool){
    assert(pPool);
    assert(*pPool);
    pr_pool_t * pool = *pPool;

    pr_mutex_lock(pool->d_lock_p);
    if(0==pool->d_instanceCount){
        pr_pool_blocklist_delete(&pool->d_blockAllocator_p);
    }
    assert(0==pool->d_instanceCount);
    pr_mutex_unlock(pool->d_lock_p);

    pr_mutex_delete(&pool->d_lock_p);

    FREE(*pPool);
}

void pr_pool_dryup(pr_pool_t* pool){
    assert(pool);
    pr_mutex_lock(pool->d_lock_p);
    pr_pool_blocklist_release(pool->d_blockAllocator_p);
    pool->d_freeList_p = NULL;
    pr_mutex_unlock(pool->d_lock_p);
}

void* pr_pool_alloc(pr_pool_t* pool){
    assert(pool);
    assert(pool->d_lock_p);
    pr_mutex_lock(pool->d_lock_p);
    if(!pool->d_freeList_p){
        pr_pool_replenish(pool);
    }
    link_t* p = pool->d_freeList_p;
    pool->d_freeList_p = p->d_next_p;
    ++pool->d_instanceCount;
    pr_mutex_unlock(pool->d_lock_p);
    return p;
}

void pr_pool_free(pr_pool_t* pool, void* ptr){
    assert(pool);
    if(ptr==NULL) return;
    assert(pool->d_lock_p);
    link_t* p = (link_t*) ptr;
    pr_mutex_lock(pool->d_lock_p);
    p->d_next_p = pool->d_freeList_p;
    pool->d_freeList_p = p;
    --pool->d_instanceCount;
    pr_mutex_unlock(pool->d_lock_p);
}

int pr_pool_hasmutex(pr_pool_t* pool){
    return 1;
}
