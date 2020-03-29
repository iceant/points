//
// Created by PCHEN on 2020-03-25.
//
#include <pr_array.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct element_s{
   int value;
}element_t;

int main(int argc, char** argv){
    pr_array_t* ary = pr_array_new(10, sizeof(element_t));
    for(int i=0; i<pr_array_length(ary); i++){
        element_t item={i};
        pr_array_put(ary, i, &item);
    }
    for(int i=0; i<pr_array_length(ary);i++){
        element_t* item = pr_array_get(ary, i);
        printf("value:%d\n", item->value);
    }
    pr_array_delete(&ary);
}

