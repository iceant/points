//
// Created by PCHEN on 2020-03-25.
//
#include <pr_pool.h>
#include <pr_thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct element_s{
   int d_value;
}element_t;


void threadFn(void* args){
    assert(args);
    pr_pool_t* pool = (pr_pool_t*)args;
    element_t* elements[1000]={0};
    for(int i=0; i<1000; i++){
        elements[i]=pr_pool_alloc(pool);
        elements[i]->d_value=i;
    }

    for(int i=0; i<1000; i++){
        printf("%ld - %d\n", pr_thread_self(), elements[i]->d_value);
        pr_pool_free(pool, elements[i]);
        pr_thread_sleep(1);
    }
    pr_thread_exit(0);
}

int main(int argc, char** argv){
    pr_pool_t* pool = pr_pool_new(1000, sizeof(element_t));

    pr_thread_t* thread1;
    pr_thread_t* thread2;

    thread1 = pr_thread_new(NULL, threadFn, pool);
    thread2 = pr_thread_new(NULL, threadFn, pool);

    pr_thread_join(thread1);
    pr_thread_join(thread2);

    pr_thread_delete(&thread1);
    pr_thread_delete(&thread2);

    pr_pool_delete(&pool);
    return 0;
}

