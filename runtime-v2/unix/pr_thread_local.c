//
// Created by PCHEN on 2020-03-26.
//
#include <pr_thread_local.h>
#include <pr_mem_util.h>
#include <pthread.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_thread_local_s{
    pthread_key_t d_key;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_local_t* pr_thread_local_new(void(*destructor)(void*)){
    pr_thread_local_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    pthread_key_create(&p->d_key, destructor);
    return p;
}

void pr_thread_local_delete(pr_thread_local_t** pTls){
    assert(pTls);
    assert(*pTls);
    pr_thread_local_t* tls = *pTls;
    pthread_key_delete(tls->d_key);
    PR_FREE(*pTls);
}

void pr_thread_local_set(pr_thread_local_t* tls, void* value){
    assert(tls);
    pthread_setspecific(tls->d_key, value);
}

void* pr_thread_local_get(pr_thread_local_t* tls){
    assert(tls);
    return pthread_getspecific(tls->d_key);
}



