//
// Created by PCHEN on 2020-03-26.
//

#include <pr_deque.h>
#include <pr_mem_util.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_deque_node_s{
    struct pr_deque_node_s * d_prev_p;
    struct pr_deque_node_s * d_next_p;
    void*                   d_value_p;
}pr_deque_node_t;

struct pr_deque_s{
    pr_deque_node_t* d_front_p;
    pr_deque_node_t* d_rare_p;
};

////////////////////////////////////////////////////////////////////////////////
pr_deque_t* pr_deque_new(){
    pr_deque_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    pr_deque_node_t* node = PR_MALLOC(sizeof(*node));
    assert(node);
    node->d_next_p = NULL;
    node->d_prev_p = NULL;
    node->d_value_p = NULL;

    p->d_front_p = p->d_rare_p = node;
    return p;
}

void pr_deque_delete(pr_deque_t** pDeque, void(*releaseFn)(void*, void*), void* args){
    assert(pDeque);
    assert(*pDeque);
    pr_deque_t* queue = *pDeque;
    if(releaseFn){
       while(queue->d_front_p->d_next_p){
           pr_deque_node_t* p = queue->d_front_p->d_next_p;
           queue->d_front_p->d_next_p = p->d_next_p;
           releaseFn(p->d_value_p, args);
           PR_FREE(p);
       }
       PR_FREE(queue->d_front_p);
    }else{
        while(queue->d_front_p){
            queue->d_rare_p = queue->d_front_p->d_next_p;
            PR_FREE(queue->d_front_p);
            queue->d_front_p = queue->d_rare_p;
        }
    }
    PR_FREE(*pDeque);
}

void pr_deque_push_back(pr_deque_t* queue, void* value){
    assert(queue);
    pr_deque_node_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_value_p = value;
    p->d_next_p = NULL;
    p->d_prev_p = queue->d_rare_p;
    queue->d_rare_p->d_next_p = p;
    queue->d_rare_p = p;
}

void* pr_deque_pop_front(pr_deque_t* queue){
    assert(queue);
    if(queue->d_front_p==queue->d_rare_p) return NULL;
    pr_deque_node_t* p = queue->d_front_p->d_next_p;
    void* value = p->d_value_p;
    queue->d_front_p->d_next_p = p->d_next_p;
    if(queue->d_rare_p==p) queue->d_rare_p = queue->d_front_p;
    PR_FREE(p);
    return value;
}

void pr_deque_push_front(pr_deque_t* queue, void* value){
    assert(queue);
    pr_deque_node_t* p = PR_MALLOC(sizeof(*p));
    p->d_prev_p = queue->d_front_p;
    p->d_next_p = queue->d_front_p->d_next_p;
    p->d_value_p = value;
    if(queue->d_rare_p==queue->d_front_p){
        //first time push
        queue->d_rare_p = p;
    }else{
        queue->d_front_p->d_next_p->d_prev_p = p;
    }
    queue->d_front_p->d_next_p = p;
}

void* pr_deque_pop_back(pr_deque_t* queue){
    assert(queue);
    if(queue->d_rare_p == queue->d_front_p) return NULL;
    pr_deque_node_t* p = queue->d_rare_p;
    void* value = p->d_value_p;
    queue->d_rare_p=p->d_prev_p;
    queue->d_rare_p->d_next_p = NULL;

    PR_FREE(p);

    return value;
}

pr_bool_t pr_deque_isempty(pr_deque_t* queue){
    assert(queue);
    return (queue->d_rare_p==queue->d_front_p)?ePR_TRUE:ePR_FALSE;
}
