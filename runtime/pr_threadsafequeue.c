//
// Created by PCHEN on 2020-03-23.
//

#include <pr_threadsafequeue.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_mem.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct queue_node_s{
    struct queue_node_s * d_next_p;
    void * d_value_p;
}queue_node_t;

struct pr_threadsafequeue_s{
    queue_node_t d_head;
    queue_node_t * d_tail_p;
    pr_mutex_t * d_lock_p;
    pr_condition_t * d_cond_p;
};

////////////////////////////////////////////////////////////////////////////////
pr_threadsafequeue_t* pr_threadsafequeue_new(){
    pr_threadsafequeue_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_head.d_next_p = NULL;
    p->d_head.d_value_p = NULL;
    p->d_tail_p = &p->d_head;
    p->d_lock_p = pr_mutex_new();
    p->d_cond_p = pr_condition_new(p->d_lock_p);
    return p;
}

void pr_threadsafequeue_delete(pr_threadsafequeue_t** pQueue, void(*releaseFn)(void*, void*), void* args){
    assert(pQueue);
    assert(*pQueue);
    pr_threadsafequeue_t* queue = *pQueue;
    pr_mutex_lock(queue->d_lock_p);
    if(releaseFn) {
        while (queue->d_head.d_next_p != NULL) {
            queue_node_t *p = queue->d_head.d_next_p;
            queue->d_head.d_next_p = p->d_next_p;
            releaseFn(p->d_value_p, args);
            FREE(p);
        }
    }else{
        while(queue->d_head.d_next_p!=NULL){
            queue_node_t* p = queue->d_head.d_next_p;
            queue->d_head.d_next_p = p->d_next_p;
            FREE(p);
        }
    }
    pr_mutex_unlock(queue->d_lock_p);
    pr_condition_delete(&queue->d_cond_p);
    pr_mutex_delete(&queue->d_lock_p);
    FREE(*pQueue);
}

void pr_threadsafequeue_push(pr_threadsafequeue_t* queue, void* value){
   assert(queue);
   queue_node_t * p = MALLOC(sizeof(*p));
   assert(p);
   p->d_next_p = NULL;
   p->d_value_p = value;
   pr_mutex_lock(queue->d_lock_p);
   queue->d_tail_p->d_next_p=p;
   queue->d_tail_p = p;
   pr_condition_notify(queue->d_cond_p);
   pr_mutex_unlock(queue->d_lock_p);
}

pr_bool_t pr_threadsafequeue_isempty(pr_threadsafequeue_t* queue){
    return (queue->d_head.d_next_p==NULL)?ePR_TRUE:ePR_FALSE;
}

void pr_threadsafequeue_pop(pr_threadsafequeue_t* queue, void** value){
    assert(queue);
    assert(value);

    pr_mutex_lock(queue->d_lock_p);
    while(queue->d_head.d_next_p==NULL){
        pr_condition_wait(queue->d_cond_p);
    }
    queue_node_t * p = queue->d_head.d_next_p;
    queue->d_head.d_next_p = p->d_next_p;
    *value = p->d_value_p;
    FREE(p);
    pr_mutex_unlock(queue->d_lock_p);
}

pr_bool_t pr_threadsafequeue_trypop(pr_threadsafequeue_t* queue, void** value){
    assert(queue);
    assert(value);

    if(queue->d_head.d_next_p==NULL) return ePR_FALSE;
    pr_bool_t ret=ePR_FALSE;
    pr_mutex_lock(queue->d_lock_p);
    if(queue->d_head.d_next_p!=NULL){
        queue_node_t * p = queue->d_head.d_next_p;
        queue->d_head.d_next_p = p->d_next_p;
        *value= p->d_value_p;
        FREE(p);
        ret = ePR_TRUE;
    }
    pr_mutex_unlock(queue->d_lock_p);
    return ret;
}