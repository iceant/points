//
// Created by PCHEN on 2020-03-17.
//
#include <pr_countdownlatch.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* threadFn(void* arg){
    pr_countdownlatch_t* latch = (pr_countdownlatch_t*)arg;
    pr_countdownlatch_await(latch);
    printf("wait is over, thread[%d] is running... \n", pthread_self());
    pthread_exit(0);
    return NULL;
}

int main(int argc, char** argv){
    pr_countdownlatch_t* latch_p = pr_countdownlatch_new(10);
    int i=0;
    pthread_t thread1, thread2;

    pthread_attr_t attr;

    pthread_create(&thread1, &attr, threadFn, latch_p);
    pthread_create(&thread2, &attr, threadFn, latch_p);

    while(i++<10){
        usleep(1000*1000);
        pr_countdownlatch_countdown(latch_p);
        printf("countdown... %d\n", i);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pr_countdownlatch_delete(&latch_p);

    return 0;
}


