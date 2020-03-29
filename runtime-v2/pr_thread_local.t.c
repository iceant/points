//
// Created by PCHEN on 2020-03-26.
//

#include <pr_thread.h>
#include <pr_thread_local.h>
#include <pr_scopeguard.h>
#include <pr_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

typedef
struct thread_context_s{
    pr_thread_local_t*  d_tls_p;
    pr_bool_t           d_is_stop;
}thread_context_t;

void threadFn(void* args){
    time_t tm;
    srand((unsigned)time(&tm));

    assert(args);
    thread_context_t* ctx = (thread_context_t*)args;
    while(!ctx->d_is_stop){
        pr_thread_local_set(ctx->d_tls_p, rand());
        printf("[%06ld] %d\n", pr_thread_self(), (unsigned)pr_thread_local_get(ctx->d_tls_p));
    }
    pr_thread_exit(0);
}

int main(int argc, char** argv){
    pr_scopeguard_t* scope = pr_scopeguard_new();

    thread_context_t context;
    context.d_tls_p = pr_thread_local_new(NULL);
    context.d_is_stop = ePR_FALSE;
    pr_scopeguard_onexit(scope, (pr_scopeguard_proc)pr_thread_local_delete, &context.d_tls_p);

    pr_thread_t* thread1 = pr_thread_new(NULL, threadFn, &context);
    pr_scopeguard_onexit(scope, (pr_scopeguard_proc)pr_thread_delete, &thread1);

    pr_thread_t* thread2 = pr_thread_new(NULL, threadFn, &context);
    pr_scopeguard_onexit(scope, (pr_scopeguard_proc)pr_thread_delete, &thread2);

    pr_thread_sleep(1000 * 2);
    context.d_is_stop = ePR_TRUE;

    pr_scopeguard_delete(&scope);

    assert(thread1==NULL);
    assert(thread2==NULL);
    assert(context.d_tls_p==NULL);
}