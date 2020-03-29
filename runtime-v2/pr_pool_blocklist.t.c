//
// Created by PCHEN on 2020-03-25.
//
#include <pr_pool_blocklist.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct element_s{
   int d_value;
}element_t;


#define ELEMENT_SIZE sizeof(element_t)

int main(int argc, char** argv){
    pr_pool_blocklist_t* list = pr_pool_blocklist_new();
    element_t* elements[10]={0};
    for(int i=0; i<10; i++){
        elements[i] = pr_pool_blocklist_alloc(list, ELEMENT_SIZE);
    }
    pr_pool_blocklist_delete(&list);
}

