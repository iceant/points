//
// Created by PCHEN on 2020-03-28.
//

#include <pr_vector.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct element_s{
    int d_value;
}element_t;

void release(void* ptr, void* args){
    printf("release %p, args:%p\n", ptr, args);
}

int main(int argc, char** argv){

    pr_vector_t* vector = pr_vector_new(10);


    element_t e1={10};
    int idx1 = 0;
    pr_vector_set(vector, idx1, &e1);
    printf("index1:%d\n", idx1);

    printf("value:%d\n", ((element_t*)pr_vector_get(vector, idx1))->d_value );

    void* ptr = pr_vector_remove(vector, idx1);
    printf("after remove: index[%d]=%p\n",idx1, pr_vector_get(vector, idx1));
    printf("removed: %p\n", ptr);

    int capacity = pr_vector_capacity(vector);
    pr_vector_set(vector, capacity-1, &e1);

    pr_vector_delete(&vector, release, NULL);

    return 0;
}