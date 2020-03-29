//
// Created by PCHEN on 2020-03-29.
//

#include <pr_thread_queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct data_s{
   int d_value;
}data_t;

void release(void* ptr, void* args){
    printf("release(%p, %p)\n", ptr, args);
}

int main(int argc, char* argv){
    pr_thread_queue_t * queue = pr_thread_queue_new();

    data_t data[10];

    for(int i=0; i<10; i++){
        data[i].d_value = i;
        pr_thread_queue_push(queue, &data[i]);
    }

    for(int i=0; i<10; i++){
        data_t* data = pr_thread_queue_pop(queue);
        printf("value:%d\n", data->d_value);
    }

    for(int i=0; i<10; i++){
        data[i].d_value = i;
        pr_thread_queue_push(queue, &data[i]);
    }

//    pr_thread_queue_clear(queue, NULL, NULL);

    pr_thread_queue_delete(&queue, NULL, NULL);
    return 0;
}