//
// Created by PCHEN on 2020-03-17.
//

#include <pr_thread.h>
#include <pr_thread_attr.h>
#include <pr_types.h>
#include <stdlib.h>
#include <stdio.h>

struct ThreadContext{
    int d_value;
    pr_bool_t d_isStop;
};

void threadFn(void* args){
   struct ThreadContext* context = (struct ThreadContext*)args;
   while(!context->d_isStop) {
       context->d_value++;
       printf("[%d] %d\n", pr_thread_self(), context->d_value);
   }
   pr_thread_exit(0);
}

int main(int argc, char** argv){
    pr_thread_t* thread1;
    pr_thread_t* thread2;

    struct ThreadContext context;
    context.d_value=0;

    thread1 = pr_thread_new(NULL, threadFn, &context);
    thread2 = pr_thread_new(NULL, threadFn, &context);

    pr_thread_sleep(1000*10);
    context.d_isStop=ePR_TRUE;

    pr_thread_join(thread1);
    pr_thread_join(thread2);

    pr_thread_delete(&thread1);
    pr_thread_delete(&thread2);

    return 0;
}
