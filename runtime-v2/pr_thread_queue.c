//
// Created by PCHEN on 2020-03-26.
//
#include <pr_thread_queue.h>
#include <pr_mem_util.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_queue_node_s{
    void*                           d_data_p;
    struct pr_thread_queue_node_s * d_next_p;
}pr_thread_queue_node_t;

struct pr_thread_queue_s{
   pr_thread_queue_node_t*  d_front_p;
   pr_thread_queue_node_t*  d_rear_p;
   pr_mutex_t*              d_lock_p;
   pr_condition_t*          d_cond_p;
   pr_bool_t                d_isStop;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_queue_t* pr_thread_queue_new(){
    pr_thread_queue_t* p = PR_MALLOC(sizeof(*p));
    assert(p);

    p->d_front_p = p->d_rear_p = (pr_thread_queue_node_t*)PR_MALLOC(sizeof(pr_thread_queue_node_t));
    assert(p->d_front_p);

    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);
    p->d_cond_p = pr_condition_new(p->d_lock_p);
    assert(p->d_cond_p);

    p->d_front_p->d_next_p = NULL;
    p->d_front_p->d_data_p = NULL;
    p->d_isStop = ePR_FALSE;

    return p;
}

void pr_thread_queue_delete(pr_thread_queue_t** pQueue, void(*releaseFn)(void*, void*), void* args){
    assert(pQueue);
    assert(*pQueue);
    pr_thread_queue_t* queue = *pQueue;
    pr_mutex_lock(queue->d_lock_p);
    if(releaseFn){
        while (queue->d_front_p->d_next_p) {
            pr_thread_queue_node_t *p = queue->d_front_p->d_next_p;
            queue->d_front_p->d_next_p = p->d_next_p;
            releaseFn(p->d_data_p, args);
            PR_FREE(p);
        }
        PR_FREE(queue->d_front_p);
        queue->d_rear_p = queue->d_front_p;
    }else {
        while (queue->d_front_p) {
            pr_thread_queue_node_t *p = queue->d_front_p;
            queue->d_rear_p = p->d_next_p;
            PR_FREE(p);
            queue->d_front_p = queue->d_rear_p;
        }
    }
    pr_mutex_unlock(queue->d_lock_p);

    pr_condition_delete(&queue->d_cond_p);
    pr_mutex_delete(&queue->d_lock_p);
    PR_FREE(*pQueue);
}

void pr_thread_queue_push(pr_thread_queue_t* queue, void* value){
    assert(queue);
    pr_thread_queue_node_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_data_p = value;
    p->d_next_p = NULL;
    pr_mutex_lock(queue->d_lock_p);
    queue->d_rear_p->d_next_p = p;
    queue->d_rear_p = p;
    pr_mutex_unlock(queue->d_lock_p);
}

void* pr_thread_queue_pop(pr_thread_queue_t* queue){
    assert(queue);
    if(queue->d_front_p==queue->d_rear_p) {
        return NULL;
    }
    pr_mutex_lock(queue->d_lock_p);
    if(queue->d_front_p==queue->d_rear_p) {
        // need to check again, the status could be changed before enter the lock
        pr_mutex_unlock(queue->d_lock_p);
        return NULL;
    }
    pr_thread_queue_node_t* p = queue->d_front_p->d_next_p;
    void* value = p->d_data_p;
    queue->d_front_p->d_next_p = p->d_next_p;
    if(queue->d_rear_p==p){
        queue->d_rear_p = queue->d_front_p;
    }
    pr_mutex_unlock(queue->d_lock_p);
    PR_FREE(p);
    return value;
}

pr_bool_t pr_thread_queue_isempty(pr_thread_queue_t* queue){
    assert(queue);
    pr_bool_t ret = ePR_FALSE;
    pr_mutex_lock(queue->d_lock_p);
    ret = (queue->d_rear_p==queue->d_front_p)?ePR_TRUE:ePR_FALSE;
    pr_mutex_unlock(queue->d_lock_p);
    return ret;
}

void pr_thread_queue_clear(pr_thread_queue_t* queue, void(*releaseFn)(void*, void*), void* args){
    assert(queue);
    pr_mutex_lock(queue->d_lock_p);
    if(releaseFn){
        while (queue->d_front_p->d_next_p) {
            pr_thread_queue_node_t *p = queue->d_front_p->d_next_p;
            queue->d_front_p->d_next_p = p->d_next_p;
            releaseFn(p->d_data_p, args);
            PR_FREE(p);
        }
        queue->d_rear_p = queue->d_front_p;
    }else {
        while (queue->d_front_p->d_next_p) {
            pr_thread_queue_node_t *p = queue->d_front_p->d_next_p;
            queue->d_front_p->d_next_p = p->d_next_p;
            PR_FREE(p);
        }
        queue->d_rear_p = queue->d_front_p;
    }
    pr_mutex_unlock(queue->d_lock_p);
}

int pr_thread_queue_sizeof(){
    return sizeof(pr_thread_queue_t);
}


