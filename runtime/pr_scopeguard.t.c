//
// Created by PCHEN on 2020-03-17.
//
#include <pr_scopeguard.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void freeProxy(void* ptr){
    printf("free [%p]\n", ptr);
    free(ptr);
}

void closeProxy(void* p){
    printf("close file [%p]\n", p);
    fclose((FILE*)p);
}

int main(int argc, char** argv){
    pr_scopeguard_t * scope = pr_scopeguard_new();
    void* ptr = malloc(1024);
    assert(ptr);
    pr_scopeguard_onexit(scope, freeProxy, ptr);

    FILE * file_p = fopen("test", "rb");
    pr_scopeguard_onexit(scope, closeProxy, file_p);

//  we can choose to dismiss the scopeguard
//    pr_scopeguard_dismiss(scope);

    if(pr_scopeguard_isdismissed(scope)) {
        fclose(file_p);
        free(ptr);
    }
    pr_scopeguard_delete(&scope);
}

