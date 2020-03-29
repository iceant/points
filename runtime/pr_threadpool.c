//
// Created by PCHEN on 2020-03-23.
//
#include <pr_threadpool.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_mem.h>
#include <pr_types.h>
#include <pr_thread.h>
#include <pr_threadlocal.h>
#include <pr_threadsafequeue.h>
#include <pr_workstealqueue.h>
#include <pr_array.h>
#include <stdlib.h>
#include <assert.h>


////////////////////////////////////////////////////////////////////////////////
typedef struct task_s{
    pr_threadpool_proc d_function;
    void* d_args;
    pr_threadpool_proc d_callback;
}task_t;

typedef struct threadholder_s{
   pr_thread_t* d_thead_p;
}threadholder_t;

struct pr_threadpool_s{
    pr_bool_t               d_done;
    pr_threadsafequeue_t*   d_poolWorkQueue;
    int                     d_threadCount;
    pr_threadlocal_t *      d_tlsindex_p;
    pr_threadlocal_t *      d_tlsLocalWorkQueue_p;
    pr_array_t *            d_queues_p;
    pr_array_t *            d_threads_p;
    pr_mutex_t *            d_lock_p;
    pr_condition_t*         d_cond_p;
};

////////////////////////////////////////////////////////////////////////////////
// static functions
static void workerThreadFn(void* args){
    assert(args);
    pr_threadpool_t* pool = (pr_threadpool_t*)args;
    unsigned myIndex = (unsigned)pr_threadlocal_get(pool->d_tlsindex_p);
    pr_threadsafequeue_t* queue = pr_array_get(pool->d_queues_p, myIndex);
    while(!pool->d_done){
        pr_threadpool_run(pool);
    }
}

static pr_bool_t popTaskFromLocalQueue(pr_threadpool_t* pool, task_t** task){
    assert(pool);
    assert(task);
    pr_threadsafequeue_t* queue = (pr_threadsafequeue_t*)pr_threadlocal_get(pool->d_tlsLocalWorkQueue_p);
    if(queue){
        return pr_threadsafequeue_trypop(queue, task);
    }else{
        return ePR_FALSE;
    }
}

static pr_bool_t popTaskFromPoolQueue(pr_threadpool_t* pool, task_t** task){
    assert(pool);
    assert(task);
    return pr_threadsafequeue_trypop(pool->d_poolWorkQueue, task);
}

static pr_bool_t popTaskFromOtherThreadQueue(pr_threadpool_t * pool, task_t** task){
    for(unsigned i=0; i<pr_array_size(pool->d_queues_p); i++){
        int myIndex = (int)pr_threadlocal_get(pool->d_tlsindex_p);
        unsigned const index = (myIndex+i+1)%pr_array_size(pool->d_queues_p);
        pr_workstealqueue_t* queue = (pr_workstealqueue_t*)pr_array_get(pool->d_queues_p, index);
        if(pr_workstealqueue_trysteal(queue, task)){
            return ePR_TRUE;
        }
    }
    return ePR_FALSE;
}
////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_WORKQUEUE_SIZE 100

////////////////////////////////////////////////////////////////////////////////
pr_threadpool_t* pr_threadpool_new(int numOfThreads){
    pr_threadpool_t * p = MALLOC(sizeof(*p));
    assert(p);
    p->d_done = ePR_FALSE;
    p->d_poolWorkQueue = pr_threadsafequeue_new();
    p->d_threadCount = numOfThreads;
    p->d_tlsLocalWorkQueue_p = pr_threadlocal_new(NULL);
    p->d_tlsindex_p = pr_threadlocal_new(NULL);
    p->d_queues_p = pr_array_new(DEFAULT_WORKQUEUE_SIZE, sizeof(task_t));
    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);
    p->d_cond_p = pr_condition_new(p->d_lock_p);
    assert(p->d_cond_p);
    return p;
}

void pr_threadpool_delete(pr_threadpool_t** pool){

}

void pr_threadpool_submit(pr_threadpool_t* pool, pr_threadpool_proc fn, void* args, pr_threadpool_proc callback){
    assert(pool);
    assert(fn);
    task_t* task = MALLOC(sizeof(*task));
    assert(task);
    task->d_function = fn;
    task->d_args = args;
    task->d_callback = callback;

    void* localWorkQueue = pr_threadlocal_get(pool->d_tlsLocalWorkQueue_p);
    if(localWorkQueue!=NULL){
        pr_threadsafequeue_push((pr_threadsafequeue_t*)localWorkQueue, task);
    }else{
        pr_threadsafequeue_push(pool->d_poolWorkQueue, task);
    }
    pr_mutex_lock(pool->d_lock_p);
    pr_condition_notify(pool->d_cond_p);
    pr_mutex_unlock(pool->d_lock_p);
}

void pr_threadpool_run(pr_threadpool_t* pool){
    task_t * task_p;
    if(popTaskFromLocalQueue(pool, &task_p) || popTaskFromPoolQueue(pool, &task_p) || popTaskFromOtherThreadQueue(pool, &task_p)){
        void* ret = task_p->d_function(task_p->d_args);
        if(task_p->d_callback){
            task_p->d_callback(ret);
        }
        FREE(task_p);
    }else{
        pr_mutex_lock(pool->d_lock_p);
        pr_condition_wait(pool->d_cond_p);
        pr_mutex_unlock(pool->d_lock_p);
    }
}


