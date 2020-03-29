//
// Created by PCHEN on 2020-03-28.
//

#include <pr_future.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pr_thread.h>

typedef struct context_s{
    int d_value;
}context_t;

void* futureFunction(void* args){
    assert(args);
    context_t* ctx = (context_t*)args;
    printf("value:%d\n", ctx->d_value++);
    printf("now, wait for 10 seconds...\n");
    pr_thread_sleep(1000*10);
    return ctx->d_value;
}

int main(int argc, char** argv){
    context_t ctx={3};

    pr_future_t* future = pr_future_new(futureFunction, &ctx);

    printf("future.get()=%d\n", pr_future_get(future));

    pr_future_delete(&future);
}