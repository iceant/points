//
// Created by PCHEN on 2020-03-17.
//
#include <pr_pool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

struct complex_s{
   int a;
};

typedef struct complex_s complex_t;

#define COMPLEX_SIZE sizeof(complex_t)

////////////////////////////////////////////////////////////////////////////////
void* threadFn(void* args){
    pr_pool_t * pool = (pr_pool_t*)args;
    complex_t* ary[10];
    for(int i=0; i<10; i++) {
        ary[i] = pr_pool_alloc(pool);
        printf("%d - alloc at:%p\n", pthread_self(), ary[i]);
    }

    for(int i=0; i<10; i++){
        printf("%d - free at:%p\n", pthread_self(), ary[i]);
        pr_pool_free(pool, ary[i]);
    }

    pthread_exit(NULL);
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv){
    pr_pool_t* pool = pr_pool_new(COMPLEX_SIZE, 100);

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, threadFn, pool);
    pthread_create(&thread2, NULL, threadFn, pool);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pr_pool_delete(&pool);
}

