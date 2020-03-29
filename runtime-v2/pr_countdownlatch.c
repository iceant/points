//
// Created by PCHEN on 2020-03-17.
//

#include <pr_countdownlatch.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_mem_util.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_countdownlatch_s{
   pr_mutex_t *         d_lock_p;
   pr_condition_t *     d_cond_p;
   int                  d_count;
};

////////////////////////////////////////////////////////////////////////////////
pr_countdownlatch_t* pr_countdownlatch_new(int count){
    assert(count>0);
    pr_countdownlatch_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_lock_p = pr_mutex_new();
    assert(p->d_lock_p);
    p->d_cond_p = pr_condition_new(p->d_lock_p);
    assert(p->d_cond_p);
    p->d_count = count;
    return p;
}

void pr_countdownlatch_delete(pr_countdownlatch_t** pLatch){
    assert(pLatch);
    assert(*pLatch);
    pr_condition_delete(&(*pLatch)->d_cond_p);
    pr_mutex_delete(&(*pLatch)->d_lock_p);
    PR_FREE(*pLatch);
}

void pr_countdownlatch_countdown(pr_countdownlatch_t* latch){
    assert(latch);
    pr_mutex_lock(latch->d_lock_p);
    latch->d_count--;
    if(0==latch->d_count){
        pr_condition_notifyall(latch->d_cond_p);
    }
    pr_mutex_unlock(latch->d_lock_p);
}

void pr_countdownlatch_await(pr_countdownlatch_t* latch){
    assert(latch);
    pr_mutex_lock(latch->d_lock_p);
    while(latch->d_count>0){
        pr_condition_wait(latch->d_cond_p);
    }
    pr_mutex_unlock(latch->d_lock_p);
}

