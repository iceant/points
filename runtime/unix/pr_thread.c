//
// Created by PCHEN on 2020-03-17.
//

#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_mem.h>
#include <pr_types.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
typedef void* (*pr_thread_proc)(void*);

struct pr_thread_s{
    pr_thread_handle_t d_handle;
    pr_thread_id_t d_theadId;
    pthread_attr_t d_attr;
    pr_bool_t d_isExisted;
    pr_bool_t d_isSuspended;
    pr_mutex_t * d_mutex_p;
    pr_condition_t* d_cond_p;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t* attr, void(*startAddress)(void*), void* args){
    pr_thread_t * p = MALLOC(sizeof(*p));
    assert(p);
    int ret = 0;
    p->d_isExisted = ePR_FALSE;
    p->d_isSuspended = ePR_FALSE;
    p->d_mutex_p = pr_mutex_new();
    assert(p->d_mutex_p);
    p->d_cond_p = pr_condition_new(p->d_mutex_p);
    assert(p->d_cond_p);

    pthread_attr_init(&p->d_attr);
    if(attr){
        int stackSize = pr_thread_attr_stacksize_get(attr);
        if(stackSize>0) {
            pthread_attr_setstacksize(&p->d_attr, stackSize);
        }
    }

    ret = pthread_create(&(p->d_handle), &p->d_attr, (pr_thread_proc)startAddress, args);
    if(ret!=0){
        pr_condition_delete(&p->d_cond_p);
        pr_mutex_delete(&p->d_mutex_p);
        FREE(p);
    }
    if(attr){
        if(ePR_TRUE==pr_thread_attr_createsuspended_get(attr)){
            pr_thread_suspend(p);
        }
    }
    return p;
}

void pr_thread_delete(pr_thread_t** pThread){
    assert(pThread);
    assert(*pThread);
    pr_thread_t * thread = *pThread;

    pr_condition_delete(&thread->d_cond_p);
    pr_mutex_delete(&thread->d_mutex_p);
    pthread_attr_destroy(&thread->d_attr);

    FREE(*pThread);
}

int pr_thread_suspend(pr_thread_t* thread){
    assert(thread);
    pr_mutex_lock(thread->d_mutex_p);
    thread->d_isSuspended = ePR_TRUE;
    pr_condition_wait(thread->d_cond_p);
    pr_mutex_unlock(thread->d_mutex_p);
    return 0;
}

int pr_thread_resume(pr_thread_t* thread){
    assert(thread);
    pr_mutex_lock(thread->d_mutex_p);
    pr_condition_notify(thread->d_cond_p);
    thread->d_isSuspended=ePR_FALSE;
    pr_mutex_unlock(thread->d_mutex_p);
    return 0;
}

int pr_thread_join(pr_thread_t* thread){
    assert(thread);
    int ret = pthread_join(thread->d_handle, NULL);
    thread->d_isExisted = ePR_TRUE;
    return ret;
}

void pr_thread_detach(pr_thread_t* thread){
    assert(thread);
    pthread_detach(thread->d_handle);
}

void pr_thread_sleep(unsigned long milliSeconds){
    struct timespec req={0}, rem={0};
    time_t sec = (int)(milliSeconds/1000);
    milliSeconds = milliSeconds - (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = milliSeconds*1000000L;
    nanosleep(&req, &rem);
}

void pr_thread_exit(int code){
    pthread_exit(&code);
}

pr_thread_id_t pr_thread_self(){
    return pthread_self();
}
