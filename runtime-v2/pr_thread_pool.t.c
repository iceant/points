//
// Created by PCHEN on 2020-03-26.
//

#include <pr_thread_pool.h>
#include <pr_thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef
struct work_context_s{
    int d_value;
}work_context_t;

void* workFn(void* args){
    assert(args);
    work_context_t* ctx = (work_context_t*)args;
    printf("[%06ld] %d\n", pr_thread_self(), ctx->d_value++);
}

void stopCallback(pr_thread_pool_t* pool, void* args){
    printf("stopped!\n");
}

int main(int argc, char** argvs){
    pr_thread_pool_t* pool = pr_thread_pool_new(10);

    work_context_t ctx={0};

    for(int i=0; i<10; i++) {
        pr_thread_pool_submit(pool, workFn, &ctx, NULL);
    }

    printf("wait for 3 seconds...\n");
    pr_thread_sleep(1000*3);
    pr_thread_pool_stop(pool, stopCallback, NULL);

    pr_thread_pool_delete(&pool);

    return 0;
}

