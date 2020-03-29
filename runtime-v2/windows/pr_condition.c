//
// Created by PCHEN on 2020-03-25.
//
#include <pr_condition.h>
#include <pr_mutex.h>
#include <pr_mem_util.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_condition_s{
    pr_mutex_t  * d_lock_p;
    CONDITION_VARIABLE d_cond;
};

////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new(pr_mutex_t* mutex){
    assert(mutex);
    pr_condition_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    InitializeConditionVariable(&p->d_cond);
    p->d_lock_p = mutex;
    return p;
}

void pr_condition_delete(pr_condition_t** pCond){
    assert(pCond);
    assert(*pCond);
    PR_FREE(*pCond);
}

void pr_condition_wait(pr_condition_t* cond){
   assert(cond);
   SleepConditionVariableCS(&cond->d_cond, (PCRITICAL_SECTION)pr_mutex_raw(cond->d_lock_p), INFINITE);
}

void pr_condition_waitfor(pr_condition_t* cond, const pr_time_t seconds){
    assert(cond);
    DWORD ms = seconds * 1000;
    SleepConditionVariableCS(&cond->d_cond, (PCRITICAL_SECTION)pr_mutex_raw(cond->d_lock_p), ms);
}

void pr_condition_notify(pr_condition_t* cond){
    assert(cond);
    WakeConditionVariable(&cond->d_cond);
}

void pr_condition_notifyall(pr_condition_t* cond){
    assert(cond);
    WakeAllConditionVariable(&cond->d_cond);
}
