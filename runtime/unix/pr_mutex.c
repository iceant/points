//
// Created by PCHEN on 2020-03-17.
//
#include <pr_mutex.h>
#include <pr_mem.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_mutex_s{
    pthread_mutex_t d_mutex;
};

////////////////////////////////////////////////////////////////////////////////
pr_mutex_t* pr_mutex_new(){
    pr_mutex_t * p = MALLOC(sizeof(*p));
    assert(p);
    pthread_mutex_init(&p->d_mutex, NULL);
    return p;
}

void pr_mutex_delete(pr_mutex_t** pMutex){
    assert(pMutex);
    assert(*pMutex);
    pthread_mutex_destroy(&(*pMutex)->d_mutex);
    FREE(*pMutex);
}

void pr_mutex_lock(pr_mutex_t* mutex){
    assert(mutex);
    pthread_mutex_lock(&mutex->d_mutex);
}

void pr_mutex_unlock(pr_mutex_t* mutex){
    assert(mutex);
    pthread_mutex_unlock(&mutex->d_mutex);
}

void* pr_mutex_raw(pr_mutex_t* mutex){
    assert(mutex);
    return &mutex->d_mutex;
}
