//
// Created by PCHEN on 2020-03-17.
//

#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <pr_mem.h>
#include <pr_types.h>
#include <windows.h>
#include <process.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef unsigned int (__stdcall (*pr_thread_proc))(void*);

struct pr_thread_s{
    pr_thread_handle_t d_handle;
    pr_thread_id_t  d_threadId;
    pr_bool_t d_isExisted;
};
////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_STACK_SIZE 0

////////////////////////////////////////////////////////////////////////////////
pr_thread_t* pr_thread_new(pr_thread_attr_t* attr, void (*startAddress)(void*), void* args){
    pr_thread_t *p = MALLOC(sizeof(*p));
    assert(p);
    p->d_handle = NULL;
    p->d_threadId=0;
    int stackSize = DEFAULT_STACK_SIZE;
    DWORD dwCreateFlag = 0;

    if(attr){
        stackSize = pr_thread_attr_stacksize_get(attr);
        if(ePR_TRUE==pr_thread_attr_createsuspended_get(attr)){
            dwCreateFlag=CREATE_SUSPENDED;
        }
    }

    HANDLE hThread = (HANDLE)_beginthreadex(NULL/*PSECURITY_DESCRIPTION*/,
              stackSize,
              (pr_thread_proc)startAddress,
              args,
              dwCreateFlag,
              (unsigned*)(&p->d_threadId)
            );
    p->d_handle = hThread;
    p->d_isExisted = ePR_FALSE;
    return p;
}

void pr_thread_delete(pr_thread_t** pThread){
    assert(pThread);
    assert(*pThread);

    pr_thread_t* thread = *pThread;
    if(thread->d_handle) {
        if(ePR_FALSE==thread->d_isExisted){
            TerminateThread(thread->d_handle, 0);
            WaitForSingleObject(thread->d_handle, INFINITE);
            thread->d_isExisted = ePR_TRUE;
        }
        CloseHandle(thread->d_handle);
        thread->d_handle = NULL;
    }

    FREE(*pThread);
}

int pr_thread_suspend(pr_thread_t* thread){
    assert(thread);
    assert(thread->d_handle);
    return SuspendThread(thread->d_handle);
}

int pr_thread_resume(pr_thread_t* thread){
    assert(thread);
    assert(thread->d_handle);
    return ResumeThread(thread->d_handle);
}

int pr_thread_join(pr_thread_t* thread){
    assert(thread);
    assert(thread->d_handle);
    int ret = WaitForSingleObject(thread->d_handle, INFINITE);
    thread->d_isExisted = ePR_TRUE;
    return ret;
}

void pr_thread_detach(pr_thread_t* thread){
    assert(thread);
    assert(thread->d_handle);
    CloseHandle(thread->d_handle);
    thread->d_handle = NULL;
}

void pr_thread_sleep(unsigned long milliSeconds){
    Sleep(milliSeconds);
}

void pr_thread_exit(int code){
    ExitThread(code);
}

pr_thread_id_t pr_thread_self(){
    return GetCurrentThreadId();
}

