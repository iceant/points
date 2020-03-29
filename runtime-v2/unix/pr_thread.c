//
// Created by PCHEN on 2020-03-25.
//

#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <pr_mem_util.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_types.h>
#include <pthread.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
typedef void*(*start_address)(void*);

struct pr_thread_s{
    pthread_t       d_handle;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t* attr, pr_thread_proc fn, void* args){
    assert(fn);
    pr_thread_t* p = PR_MALLOC(sizeof(*p));
    assert(p);

    pthread_attr_t d_attr;
    pthread_attr_init(&d_attr);

    if(attr){
        pthread_attr_setstacksize(&d_attr, pr_thread_attr_stacksize_get(attr));
        if(pr_thread_attr_detach_get(attr)){
            pthread_attr_setdetachstate(&d_attr, PTHREAD_CREATE_DETACHED);
        }else{
            pthread_attr_setdetachstate(&d_attr, PTHREAD_CREATE_JOINABLE);
        }
    }

    pthread_create(&p->d_handle, &d_attr, (start_address)fn, args);

    return p;
}

void pr_thread_delete(pr_thread_t** pThread){
    assert(pThread);
    assert(*pThread);
    PR_FREE(*pThread);
}

pr_status_t pr_thread_join(pr_thread_t* handle){
    assert(handle);
    return pthread_join(handle->d_handle, (void**)NULL);
}

pr_status_t pr_thread_detach(pr_thread_t* handle){
    assert(handle);
    return pthread_detach(handle->d_handle);
}

void pr_thread_sleep(unsigned long ms){
    struct timespec req={0}, rem={0};
    time_t sec = (int)(ms/1000);
    ms = ms - (sec * 1000);
    req.tv_sec = sec;
    req.tv_nsec = ms*1000000L;
    nanosleep(&req, &rem);
}


pr_thread_id_t pr_thread_self(){
    return pthread_self();
}

void pr_thread_exit(int code){
    pthread_exit((void*)code);
}

int pr_thread_hardware_concurrency(){
    return sysconf(_SC_NPROCESSORS_ONLN);
}

