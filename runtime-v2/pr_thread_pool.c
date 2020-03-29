//
// Created by PCHEN on 2020-03-29.
//
#include <pr_thread_pool.h>
#include <pr_mem_util.h>
#include <pr_vector.h>
#include <pr_thread_queue.h>
#include <pr_countdownlatch.h>
#include <pr_thread.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_thread_pool_task_s{
    pr_thread_pool_proc d_function;
    void* d_args;
    pr_thread_pool_callback d_callback;
}pr_thread_pool_task_t;

struct pr_thread_pool_s{
    int d_numOfThreads;
    pr_bool_t d_isStop;

    pr_mutex_t * d_lock_p;

    pr_thread_queue_t* d_taskQueue_p;
    pr_vector_t* d_threads_p;

    pr_countdownlatch_t* d_latch_p;
    pr_thread_pool_stopCallback d_stopCallback;
    void* d_stopArgs;
};

////////////////////////////////////////////////////////////////////////////////
// static functions
static void taskDeleteFn(void* task, void* args){
    assert(task);
    pr_thread_pool_task_t* task_p = (pr_thread_pool_task_t*)task;
    task_p->d_function = NULL;
    task_p->d_args = NULL;
    task_p->d_callback = NULL;
    PR_FREE(task_p);
}
static void threadDeleteFn(void* pthread, void* args){
    assert(pthread);
    pr_thread_t* thread = (pr_thread_t*)pthread;
    pr_thread_delete(&thread);
}

static void pr_thread_pool__threadFn(void* args){
    assert(args);
    pr_thread_pool_t* pool = (pr_thread_pool_t*)args;

    while(!pool->d_isStop){
        pr_thread_pool_task_t* task = (pr_thread_pool_task_t*)pr_thread_queue_pop(pool->d_taskQueue_p);
        if(task && task->d_function){
            void* ret = task->d_function(task->d_args);
            if(task->d_callback){
                task->d_callback(pool, ret);
            }
            PR_FREE(task);
        }
    }

    //exit
    pr_countdownlatch_countdown(pool->d_latch_p);
    pr_thread_exit(0);
}

////////////////////////////////////////////////////////////////////////////////
// pool functions
pr_thread_pool_t* pr_thread_pool_new(int numOfThreads){
    assert(numOfThreads>0);
    pr_thread_pool_t* p = PR_MALLOC(sizeof(*p));
    assert(p);

    p->d_numOfThreads= numOfThreads;
    p->d_isStop = ePR_FALSE;

    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);

    p->d_latch_p = pr_countdownlatch_new(numOfThreads);

    p->d_taskQueue_p = pr_thread_queue_new();
    assert(p->d_taskQueue_p);

    p->d_stopCallback = NULL;

    p->d_threads_p = pr_vector_new(numOfThreads);
    for(int i=0; i<numOfThreads; i++){
        pr_thread_t* t = pr_thread_new(NULL, pr_thread_pool__threadFn, p);
        pr_vector_set(p->d_threads_p, i, t);
    }

    return p;
}

void pr_thread_pool_delete(pr_thread_pool_t** pPool){
    assert(pPool);
    assert(*pPool);

    pr_thread_pool_t* pool = *pPool;

    pr_countdownlatch_await(pool->d_latch_p);
    // wait for all threads exited
    if(pool->d_stopCallback){
        pool->d_stopCallback(pool, pool->d_stopArgs);
    }

    pr_mutex_lock(pool->d_lock_p);
    pr_countdownlatch_delete(&pool->d_latch_p);
    pr_vector_delete(&pool->d_threads_p, threadDeleteFn, NULL);
    pr_thread_queue_delete(&pool->d_taskQueue_p, taskDeleteFn, NULL);
    pr_mutex_unlock(pool->d_lock_p);

    pr_mutex_delete(&pool->d_lock_p);
    PR_FREE(*pPool);
}

void pr_thread_pool_submit(pr_thread_pool_t* pool, pr_thread_pool_proc proc, void* args, pr_thread_pool_callback cb){
    assert(pool);
    assert(proc);
    pr_thread_pool_task_t * p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_function = proc;
    p->d_args = args;
    p->d_callback = cb;

    pr_thread_queue_push(pool->d_taskQueue_p, p);

}

void pr_thread_pool_stop(pr_thread_pool_t* pool, pr_thread_pool_stopCallback stopCallback, void* args){
    assert(pool);
    assert(stopCallback);

    pr_mutex_lock(pool->d_lock_p);
    pool->d_stopCallback = stopCallback;
    pool->d_stopArgs = args;
    pool->d_isStop = ePR_TRUE;
    pr_mutex_unlock(pool->d_lock_p);
}
