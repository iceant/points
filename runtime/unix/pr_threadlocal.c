//
// Created by PCHEN on 2020-03-20.
//

#include <pr_threadlocal.h>
#include <pr_mem.h>
#include <pthread.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_threadlocal_s{
   pthread_key_t d_key;
};

////////////////////////////////////////////////////////////////////////////////
pr_threadlocal_t* pr_threadlocal_new(void(*dest)(void*)){
    pr_threadlocal_t* p = MALLOC(sizeof(*p));
    assert(p);
    pthread_key_create(&p->d_key, dest);
    return p;
}

void pr_threadlocal_delete(pr_threadlocal_t** pPrThreadlocal){
    assert(pPrThreadlocal);
    assert(*pPrThreadlocal);
    pthread_key_delete((*pPrThreadlocal)->d_key);
    FREE(*pPrThreadlocal);
}

void pr_threadlocal_set(pr_threadlocal_t* threadlocal, void* value){
    assert(threadlocal);
    pthread_setspecific(threadlocal->d_key, value);
}

void* pr_threadlocal_get(pr_threadlocal_t* threadlocal){
    assert(threadlocal);
    return pthread_getspecific(threadlocal->d_key);
}
