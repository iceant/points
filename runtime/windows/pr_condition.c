//
// Created by PCHEN on 2020-03-17.
//

#include <pr_condition.h>
#include <pr_mutex.h>
#include <pr_mem.h>
#include <assert.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_condition_s{
    pr_mutex_t * d_lock_p;
    CONDITION_VARIABLE d_cond;
};

////////////////////////////////////////////////////////////////////////////////
pr_condition_t* pr_condition_new(pr_mutex_t * lock){
    assert(lock);
    pr_condition_t* p = MALLOC(sizeof(*p));
    assert(p);
    InitializeConditionVariable(&p->d_cond);
    p->d_lock_p = lock;
    return p;
}

void pr_condition_delete(pr_condition_t** cond){
    assert(cond);
    assert(*cond);
    FREE(*cond);
}

void pr_condition_wait(pr_condition_t* cond){
    assert(cond);
    SleepConditionVariableCS(&cond->d_cond, (PCRITICAL_SECTION)pr_mutex_raw(cond->d_lock_p), INFINITE);
}

void pr_condition_waitfor(pr_condition_t* cond, const time_t seconds){
    assert(cond);
    DWORD milliseconds = seconds * 1000;
    SleepConditionVariableCS(&cond->d_cond, (PCRITICAL_SECTION)pr_mutex_raw(cond->d_lock_p), milliseconds);
}

void pr_condition_notify(pr_condition_t* cond){
    assert(cond);
    WakeConditionVariable(&cond->d_cond);
}

void pr_condition_notifyall(pr_condition_t* cond){
    assert(cond);
    WakeAllConditionVariable(&cond->d_cond);
}