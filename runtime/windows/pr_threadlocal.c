//
// Created by PCHEN on 2020-03-20.
//

#include <pr_threadlocal.h>
#include <pr_mem.h>
#include <windows.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef void (*pr_threadlocal_destructor_function)(void*);

struct pr_threadlocal_s{
    DWORD d_key;
    pr_threadlocal_destructor_function d_destructor;
};

////////////////////////////////////////////////////////////////////////////////
pr_threadlocal_t * pr_threadlocal_new(void(*dest)(void*)){
   pr_threadlocal_t* p = MALLOC(sizeof(*p));
   assert(p);
   p->d_key = TlsAlloc();
   p->d_destructor = dest;
   return p;
}

void pr_threadlocal_delete(pr_threadlocal_t** pPrThreadlocal){
    assert(pPrThreadlocal);
    assert(*pPrThreadlocal);
    pr_threadlocal_t* threadlocal = *pPrThreadlocal;
    if(threadlocal->d_destructor){
        void* value = TlsGetValue(threadlocal->d_key);
        threadlocal->d_destructor(value);
    }
    TlsFree(threadlocal->d_key);
    FREE(*pPrThreadlocal);
}

void pr_threadlocal_set(pr_threadlocal_t* threadlocal, void* value){
    assert(threadlocal);
    TlsSetValue(threadlocal->d_key, value);
}

void* pr_threadlocal_get(pr_threadlocal_t* threadlocal){
    assert(threadlocal);
    return TlsGetValue(threadlocal->d_key);
}
