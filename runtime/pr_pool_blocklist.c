//
// Created by PCHEN on 2020-03-17.
//

#include <pr_pool_blocklist.h>
#include <pr_mem.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_pool_blocklink_s{
   void* d_block_p;
   struct pr_pool_blocklink_s * d_next_p;
};

typedef struct pr_pool_blocklink_s pr_pool_blocklink_t;

struct pr_pool_blocklist_s{
   pr_pool_blocklink_t * d_blockList_p;
};

////////////////////////////////////////////////////////////////////////////////
// blocklist
pr_pool_blocklist_t * pr_pool_blocklist_new(){
    pr_pool_blocklist_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_blockList_p = NULL;
    return p;
}

void pr_pool_blocklist_delete(pr_pool_blocklist_t** pp){
    assert(pp);
    assert(*pp);
    pr_pool_blocklist_t* p = *pp;
    while(p->d_blockList_p){
        pr_pool_blocklink_t* q = p->d_blockList_p;
        p->d_blockList_p = q->d_next_p;
        FREE(q);
    }
    FREE(*pp);
}

void pr_pool_blocklist_release(pr_pool_blocklist_t* p){
    assert(p);
    while(p->d_blockList_p){
        pr_pool_blocklink_t* q = p->d_blockList_p;
        p->d_blockList_p = q->d_next_p;
        FREE(q);
    }
}

void* pr_pool_blocklist_alloc(pr_pool_blocklist_t * t, int nbytes){
    assert(t);
    pr_pool_blocklink_t * link = MALLOC(sizeof(*link));
    assert(link);
    link->d_block_p = MALLOC(nbytes);
    assert(link->d_block_p);
    link->d_next_p = t->d_blockList_p;
    t->d_blockList_p = link;
    return link->d_block_p;
}

