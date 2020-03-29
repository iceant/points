//
// Created by PCHEN on 2020-03-25.
//
#include <pr_mutex.h>
#include <pr_mem_util.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_mutex_s{
    CRITICAL_SECTION d_mutex;
};

////////////////////////////////////////////////////////////////////////////////
pr_mutex_t* pr_mutex_new(){
    pr_mutex_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    InitializeCriticalSection(&p->d_mutex);
    return p;
}

void pr_mutex_delete(pr_mutex_t** pMutex){
    assert(pMutex);
    assert(*pMutex);
    DeleteCriticalSection(&(*pMutex)->d_mutex);
    PR_FREE(*pMutex);
}

void pr_mutex_lock(pr_mutex_t* mutex){
    assert(mutex);
    EnterCriticalSection(&mutex->d_mutex);
}

void pr_mutex_unlock(pr_mutex_t* mutex){
    assert(mutex);
    LeaveCriticalSection(&mutex->d_mutex);
}

void* pr_mutex_raw(pr_mutex_t* mutex){
    assert(mutex);
    return &mutex->d_mutex;
}



