//
// Created by PCHEN on 2020-03-23.
//
#include <pr_workstealqueue.h>
#include <pr_types.h>
#include <pr_mem.h>
#include <pr_mutex.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct dqueue_node_s{
    struct dqueue_node_s *  d_prev_p;
    struct dqueue_node_s *  d_next_p;
    void                 *  d_value_p;
}dqueue_node_t;

typedef struct dqueue_s{
    dqueue_node_t d_head;
    dqueue_node_t * d_tail_p;
}dqueue_t;

struct pr_workstealqueue_s{
    dqueue_t *  d_queue_p;
    pr_mutex_t* d_lock_p;
};

////////////////////////////////////////////////////////////////////////////////
// dqueue
static void dqueue_init(dqueue_t* queue){
   assert(queue);
   queue->d_head.d_next_p=NULL;
   queue->d_head.d_value_p = NULL;
   queue->d_tail_p = &queue->d_head;
}

static void dqueue_push(dqueue_t* queue, void* value){
    assert(queue);
    dqueue_node_t * n = MALLOC(sizeof(*n));
    assert(n);
    n->d_value_p = value;
    n->d_next_p = NULL;
    n->d_prev_p = queue->d_tail_p;
    queue->d_tail_p->d_next_p = n;
    queue->d_tail_p = n;
}

static void dqueue_pop(dqueue_t* queue, void** value){
   assert(queue);
   assert(value);
   dqueue_node_t* n = queue->d_head.d_next_p;
   *value = n->d_value_p;
   queue->d_head.d_next_p = n->d_next_p;
   n->d_next_p->d_prev_p = &queue->d_head;
   FREE(n);
}

static void dqueue_back(dqueue_t* queue, void** value){
    assert(queue);
    assert(value);

    dqueue_node_t* p = queue->d_tail_p;
    *value = p->d_value_p;
    p->d_prev_p->d_next_p = p->d_next_p;
    FREE(p);
}

static void dqueue_clear(dqueue_t* queue, void(*releaseFn)(void*, void*), void* args){
    assert(queue);
    if(releaseFn){
        dqueue_node_t* p=NULL;
        while(queue->d_head.d_next_p){
           p = queue->d_head.d_next_p;
           queue->d_head.d_next_p = p->d_next_p;
           releaseFn(p->d_value_p, args);
           FREE(p);
        }
    }else{
        dqueue_node_t* p = NULL;
        while(queue->d_head.d_next_p){
            p = queue->d_head.d_next_p;
            queue->d_head.d_next_p = p->d_next_p;
            FREE(p);
        }
    }
    queue->d_tail_p = &queue->d_head;
}
////////////////////////////////////////////////////////////////////////////////
pr_workstealqueue_t* pr_workstealqueue_new(){
    pr_workstealqueue_t* p = MALLOC(sizeof(*p));
    assert(p);

    dqueue_t * queue = MALLOC(sizeof(*p->d_queue_p));
    assert(queue);
    dqueue_init(queue);
    p->d_queue_p = queue;

    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);
    return p;
}

void pr_workstealqueue_delete(pr_workstealqueue_t** pQueue, void(*releaseFn)(void*, void*), void* args){
    assert(pQueue);
    assert(*pQueue);

    pr_workstealqueue_t* queue = *pQueue;
    dqueue_clear(queue->d_queue_p, releaseFn, args);
    FREE(queue->d_queue_p);

    pr_mutex_delete(&queue->d_lock_p);

    FREE(*pQueue);
}

void pr_workstealqueue_push(pr_workstealqueue_t* queue, void* data){
    assert(queue);
    pr_mutex_lock(queue->d_lock_p);
    dqueue_push(queue->d_queue_p, data);
    pr_mutex_unlock(queue->d_lock_p);
}

pr_bool_t pr_workstealqueue_isempty(pr_workstealqueue_t* queue){
    assert(queue);
    assert(queue->d_queue_p);
    return (queue->d_queue_p->d_head.d_next_p==NULL)?ePR_TRUE:ePR_FALSE;
}

pr_bool_t pr_workstealqueue_trypop(pr_workstealqueue_t* queue, void** ret){
    assert(queue);
    assert(ret);

    pr_bool_t bRet=ePR_FALSE;
    pr_mutex_lock(queue->d_lock_p);
    if(queue->d_queue_p->d_head.d_next_p!=NULL){
        dqueue_pop(queue, ret);
        bRet = ePR_TRUE;
    }
    pr_mutex_unlock(queue->d_lock_p);
    return bRet;
}

pr_bool_t pr_workstealqueue_trysteal(pr_workstealqueue_t* queue, void** data){
    assert(queue);
    assert(data);

    pr_bool_t ret = ePR_FALSE;
    pr_mutex_lock(queue->d_lock_p);
    if(queue->d_queue_p->d_head.d_next_p!=NULL){
        dqueue_back(queue, data);
        ret = ePR_TRUE;
    }
    pr_mutex_unlock(queue->d_queue_p);
    return ret;
}
