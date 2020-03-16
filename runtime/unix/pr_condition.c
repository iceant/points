//
// Created by PCHEN on 2020-03-17.
//
#include <pr_condition.h>
#include <pr_mutex.h>
#include <pr_mem.h>
#include <assert.h>
#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_condition_s{
   pr_mutex_t * d_lock_p;
   pthread_cond_t d_cond;
};


////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new(pr_mutex_t* lock){
    assert(lock);
    pr_condition_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_lock_p = lock;
    pthread_cond_init(&p->d_cond, NULL);
    return p;
}

void pr_condition_delete(pr_condition_t** pCond){
    assert(pCond);
    assert(*pCond);
    pthread_cond_destroy(&((*pCond)->d_cond));
    FREE(*pCond);
}

void pr_condition_wait(pr_condition_t* cond){
    assert(cond);
    pthread_cond_wait(&cond->d_cond, (pthread_mutex_t*)pr_mutex_raw(cond->d_lock_p));
}

void pr_condition_wraitfor(pr_condition_t* cond, const time_t seconds){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec+=seconds;
    pthread_cond_timedwait(&cond->d_cond, (pthread_mutex_t*)pr_mutex_raw(cond->d_lock_p), &ts);
}

void pr_condition_notify(pr_condition_t* cond){
    pthread_cond_signal(&cond->d_cond);
}

void pr_condition_notifyall(pr_condition_t* cond){
    pthread_cond_broadcast(&cond->d_cond);
}
