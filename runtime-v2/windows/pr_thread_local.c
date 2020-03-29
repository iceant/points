//
// Created by PCHEN on 2020-03-26.
//
#include <pr_thread_local.h>
#include <pr_mem_util.h>
#include <pr_types.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////
typedef void (*destructor_proc)(void*);
struct pr_thread_local_s{
   DWORD            d_tlsIndex;
   destructor_proc  d_destructor_fn;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_local_t* pr_thread_local_new(void (*destructor)(void*)){
    pr_thread_local_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_tlsIndex = TlsAlloc();
    p->d_destructor_fn = destructor;
    return p;
}

void pr_thread_local_delete(pr_thread_local_t** pTls){
    assert(pTls);
    assert(*pTls);
    pr_thread_local_t* tls = *pTls;
    if(tls->d_destructor_fn){
        void* value = TlsGetValue(tls->d_tlsIndex);
        tls->d_destructor_fn(value);
    }
    TlsFree(tls->d_tlsIndex);
    PR_FREE(*pTls);
}

void pr_thread_local_set(pr_thread_local_t* tls, void* value){
    assert(tls);
    TlsSetValue(tls->d_tlsIndex, value);
}

void* pr_thread_local_get(pr_thread_local_t* tls){
    assert(tls);
    return TlsGetValue(tls->d_tlsIndex);
}

