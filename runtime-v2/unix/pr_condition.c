//
// Created by PCHEN on 2020-03-25.
//
#include <pr_condition.h>
#include <pr_mutex.h>
#include <pr_mem_util.h>
#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_condition_s{
    pr_mutex_t* d_lock_p;
    pthread_cond_t d_cond;
};

////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new(pr_mutex_t * mutex){
    assert(mutex);
    pr_condition_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    pthread_cond_init(&p->d_cond, NULL);
    p->d_lock_p = mutex;
    return p;
}

void pr_condition_delete(pr_condition_t** pCond){
    assert(pCond);
    assert(*pCond);
    pthread_cond_destroy(&(*pCond)->d_cond);
    PR_FREE(*pCond);
}

void pr_condition_wait(pr_condition_t* cond){
    assert(cond);
    pthread_cond_wait(&cond->d_cond, (pthread_mutex_t*)pr_mutex_raw(cond->d_lock_p));
}

void pr_condition_waitfor(pr_condition_t* cond, const pr_time_t seconds){
    assert(cond);
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec+=seconds;
    pthread_cond_timedwait(&cond->d_cond, (pthread_mutex_t*)pr_mutex_raw(cond->d_lock_p), &ts);
}

void pr_condition_notify(pr_condition_t* cond){
    assert(cond);
    pthread_cond_signal(&cond->d_cond);
}

void pr_condition_notifyall(pr_condition_t* cond){
    assert(cond);
    pthread_cond_broadcast(&cond->d_cond);
}

