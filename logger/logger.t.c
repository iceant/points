//
// Created by PCHEN on 2020-03-20.
//
#include <logger.h>
#include <pr_thread.h>
#include <pr_mutex.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void stopCallback(logger_t* logger, void* args){
    printf("logger stopped!\n");
}

void writeThreadFn(void* args) {
    printf("[%06ld] Writer Thread created!\n", pr_thread_self());
    assert(args);
    logger_t *logger = (logger_t *) args;
    int i=0;
    while (logger_isrunning(logger)) {
        i = i+1;
        LOGGER_INFO(logger, "[%09d] %s", i, "Hello, Chen Peng, 中文!");
    }
    printf("[%06ld] Stopping writerThread... sended %d messages\n", pr_thread_self(), i);
    pr_thread_exit(0);
}

void stopThreadFn(void* args){
    logger_t* logger = (logger_t*)args;
    pr_thread_sleep(1000*1);
    printf("try stop...\n");
    logger_stop(logger, stopCallback, NULL);
    pr_thread_exit(0);
}

void startCallback(logger_t* logger, void* args){
   printf("logger started!\n");
}
int main(int argc, char** argv){
    logger_t* logger = logger_new("d:\\test.log", eLOGGER_LEVEL_INFO);

    logger_start(logger, startCallback, NULL);

    pr_thread_t* thread1 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* thread2 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* thread3 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* thread4 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* thread5 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* thread6 = pr_thread_new(NULL, writeThreadFn, logger);
//    pr_thread_t* thread7 = pr_thread_new(NULL, writeThreadFn, logger);
//    pr_thread_t* thread8 = pr_thread_new(NULL, writeThreadFn, logger);
    pr_thread_t* stopThread = pr_thread_new(NULL, stopThreadFn, logger);

    pr_thread_join(stopThread);
    pr_thread_join(thread1);
    pr_thread_join(thread2);
    pr_thread_join(thread3);
    pr_thread_join(thread4);
    pr_thread_join(thread5);
    pr_thread_join(thread6);
//    pr_thread_join(thread7);
//    pr_thread_join(thread8);

    pr_thread_delete(&stopThread);
    pr_thread_delete(&thread1);
    pr_thread_delete(&thread2);
    pr_thread_delete(&thread3);
    pr_thread_delete(&thread4);
    pr_thread_delete(&thread5);
    pr_thread_delete(&thread6);
//    pr_thread_delete(&thread7);
//    pr_thread_delete(&thread8);

    logger_delete(&logger);

}

