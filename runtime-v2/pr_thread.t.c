//
// Created by PCHEN on 2020-03-26.
//
#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pr_types.h>

typedef struct thread_context_s{
   int          d_value;
   pr_bool_t    d_isstop;
}thread_context_t;

void threadFn(void* args){
    assert(args);
    thread_context_t* context = (thread_context_t*)args;
    while(!context->d_isstop) {
        printf("%06ld - %d\n", pr_thread_self(), context->d_value++);
        pr_thread_sleep(300);
    }
    pr_thread_exit(0);
}

int main(int argc, char** argv){
    pr_thread_t* thread1;
    pr_thread_t* thread2;

    pr_thread_attr_t* attr = pr_thread_attr_new();
    pr_thread_attr_detach_set(attr, ePR_TRUE);

    thread_context_t context= {0, ePR_FALSE};

    thread1 = pr_thread_new(NULL, threadFn, &context);
    thread2 = pr_thread_new(attr, threadFn, &context);

    pr_thread_sleep(1000*10);
    context.d_isstop=ePR_TRUE;

    pr_thread_join(thread1);
    pr_thread_join(thread2);

    pr_thread_attr_delete(&attr);
    pr_thread_delete(&thread1);
    pr_thread_delete(&thread2);

    printf("hardware_concurrency:%d\n", pr_thread_hardware_concurrency());
}

