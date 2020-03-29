//
// Created by PCHEN on 2020-03-25.
//
#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <pr_mem_util.h>
#include <pr_types.h>
#include <windows.h>
#include <process.h>

////////////////////////////////////////////////////////////////////////////////
typedef unsigned ( __stdcall *start_address )( void * );

struct pr_thread_s{
    HANDLE d_handle;
    unsigned  d_threadId;
};

////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t* attr, pr_thread_proc fn, void* args){
    pr_thread_t* p = PR_MALLOC(sizeof(*p));
    assert(p);

    unsigned initFlag = 0;
    if(attr) {
        if(pr_thread_attr_stacksizeparamisreservation_get(attr)){
            initFlag = STACK_SIZE_PARAM_IS_A_RESERVATION;
        }
    }

    p->d_handle = (pr_thread_t*)_beginthreadex(NULL,
                                    (DWORD)(attr?pr_thread_attr_stacksize_get(attr):0),
                                    (start_address)fn,
                                    args,
                                    initFlag,
                                    &p->d_threadId );

    if(attr && pr_thread_attr_detach_get(attr)){
        CloseHandle(p->d_handle);
        p->d_handle = NULL;
    }
    return p;
}

void pr_thread_delete(pr_thread_t** pThread){
    assert(pThread);
    assert(*pThread);
    pr_thread_t* thread = *pThread;
    if(thread->d_handle){
        CloseHandle(thread->d_handle);
        thread->d_handle = NULL;
    }
    PR_FREE(*pThread);
}

pr_status_t pr_thread_join(pr_thread_t* thread){
    assert(thread);
    return WaitForSingleObject(thread->d_handle, INFINITE);
}

pr_status_t pr_thread_detach(pr_thread_t* handle){
    assert(handle);
    int ret = CloseHandle(handle->d_handle);
    handle->d_handle = NULL;
    return ret;
}

void pr_thread_sleep(unsigned long milliseconds){
    Sleep(milliseconds);
}

pr_thread_id_t pr_thread_self(){
    return GetCurrentThreadId();
}

void pr_thread_exit(int code){
    _endthreadex(code);
}

int pr_thread_hardware_concurrency(){
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
}