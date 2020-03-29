//
// Created by PCHEN on 2020-03-26.
//
#include <pr_deque.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void release(void* value, void* args){

}

int main(int argc, char** argv){
    pr_deque_t* queue = pr_deque_new();

    for(int i=0; i<10; i++){
       pr_deque_push_back(queue, i);
    }

    for(int i=0; i<10; i++){
        printf("%d\n", pr_deque_pop_back(queue));
    }

    printf("front...\n");

    for(int i=0; i<10; i++){
        pr_deque_push_front(queue, i);
    }

    for(int i=0; i<10; i++){
        printf("%d\n", pr_deque_pop_front(queue));
    }

    printf("push front/pop back...\n");

    for(int i=0; i<10; i++){
        pr_deque_push_front(queue, i);
    }

    for(int i=0; i<10; i++){
        printf("%d\n", pr_deque_pop_back(queue));
    }

    printf("push back/pop front...\n");

    for(int i=0; i<10; i++){
        pr_deque_push_back(queue, i);
    }

    for(int i=0; i<10; i++){
        printf("%d\n", pr_deque_pop_front(queue));
    }

    printf("isEmpty?%s\n", pr_deque_isempty(queue)?"TRUE":"FALSE");

    pr_deque_delete(&queue, release, NULL);

    return 0;
}

