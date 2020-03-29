//
// Created by PCHEN on 2020-03-25.
//
#include <pr_pool_blocklist.h>
#include <pr_mem_util.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct blocklink_s {
    void* d_block_p;
    struct blocklink_s * d_next_p;
}blocklink_t;

struct pr_pool_blocklist_s{
    blocklink_t* d_blocklist_p;
};

////////////////////////////////////////////////////////////////////////////////
pr_pool_blocklist_t* pr_pool_blocklist_new(){
    pr_pool_blocklist_t* p;
    PR_NEW(p);
    p->d_blocklist_p = NULL;
    return p;
}

void pr_pool_blocklist_delete(pr_pool_blocklist_t** pList){
    assert(pList);
    assert(*pList);
    pr_pool_blocklist_t* list = *pList;
    while(list->d_blocklist_p){
        blocklink_t* p = list->d_blocklist_p;
        list->d_blocklist_p = p->d_next_p;
        PR_FREE(p->d_block_p);
        PR_FREE(p);
    }
    PR_FREE(*pList);
}

void* pr_pool_blocklist_alloc(pr_pool_blocklist_t* list, int nBytes){
    assert(list);
    assert(nBytes>0);
    blocklink_t * link = PR_MALLOC(sizeof(*link));
    assert(link);
    link->d_block_p = PR_MALLOC(nBytes);
    assert(link->d_block_p);
    link->d_next_p = list->d_blocklist_p;
    list->d_blocklist_p = link;
    return link->d_block_p;
}

void pr_pool_blocklist_release(pr_pool_blocklist_t* list){
    assert(list);
    while(list->d_blocklist_p){
        blocklink_t* p = list->d_blocklist_p;
        list->d_blocklist_p = p->d_next_p;
        PR_FREE(p->d_block_p);
        PR_FREE(p);
    }
}



