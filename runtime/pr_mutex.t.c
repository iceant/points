//
// Created by PCHEN on 2020-03-17.
//
#include <pr_mutex.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

typedef struct object_s{
   int count;
   pr_mutex_t * d_mutex_p;
}object_t;

void* threadFn(void* args){
    object_t* object = (object_t*)args;
    while(object->count<10) {
        pr_mutex_lock(object->d_mutex_p);
        object->count++;
        printf("%d - %d\n", pthread_self(), object->count);
        pr_mutex_unlock(object->d_mutex_p);
        usleep(1000*600);
    }
    pthread_exit(0);
}

int main(int argc, char** argv){
    object_t * object = malloc(sizeof(*object));
    assert(object);
    object->count = 0;
    object->d_mutex_p = pr_mutex_new();
    assert(object->d_mutex_p);

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1,NULL, threadFn, object);
    pthread_create(&thread2,NULL, threadFn, object);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pr_mutex_delete(&object->d_mutex_p);
    free(object);
    object = NULL;
    return 0;
}

