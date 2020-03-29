//
// Created by PCHEN on 2020-03-28.
//

#include <pr_vector.h>
#include <pr_mem_util.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_vector_link_s{
    struct pr_vector_link_s* d_link_p;
    void* d_value_p;
}pr_vector_link_t;

struct pr_vector_s{
    pr_vector_link_t* d_list_p;
    int   d_size;
    int   d_capacity;
};

#define PR_VECTOR_ELEMENT_SIZE sizeof(struct pr_vector_link_s)

////////////////////////////////////////////////////////////////////////////////
pr_vector_t* pr_vector_new(int capacity){
    assert(capacity>0);
    pr_vector_t* p = PR_MALLOC(sizeof(*p));
    assert(p);

    p->d_capacity = capacity;
    p->d_size = 0;
    char* start = PR_MALLOC(PR_VECTOR_ELEMENT_SIZE * capacity);
    assert(start);
    char* end = &start[(capacity-1)*PR_VECTOR_ELEMENT_SIZE];

    for(char* q = start; q<end; q+=PR_VECTOR_ELEMENT_SIZE){
        ((pr_vector_link_t*)q)->d_link_p = (pr_vector_link_t*)(q+PR_VECTOR_ELEMENT_SIZE);
        ((pr_vector_link_t*)q)->d_value_p = NULL;
    }
    ((pr_vector_link_t*)end)->d_link_p = NULL;
    ((pr_vector_link_t*)end)->d_value_p = NULL;

    p->d_list_p = (pr_vector_link_t*)start;
    return p;
}

void pr_vector_delete(pr_vector_t** pVector, void (*releaseFn)(void*, void*), void* args){
    assert(pVector);
    assert(*pVector);

    pr_vector_t* vector = *pVector;
    if(releaseFn) {
        while (vector->d_list_p) {
            pr_vector_link_t *p = vector->d_list_p;
            vector->d_list_p = p->d_link_p;
            releaseFn(p->d_value_p, args);
        }
    }
    PR_FREE(vector->d_list_p);
    PR_FREE(*pVector);
}

void pr_vector_set(pr_vector_t* vector, int index, void* value){
    assert(vector);
    assert(index>=0 && index<vector->d_capacity);
    pr_vector_link_t* link = (pr_vector_link_t*)(vector->d_list_p + index );
    link->d_value_p = value;
}

void* pr_vector_get(pr_vector_t* vector, int index){
    assert(vector);
    assert(index>=0 && index < vector->d_capacity);
    pr_vector_link_t* link = (pr_vector_link_t*)(vector->d_list_p + index);
    return link->d_value_p;
}

void* pr_vector_remove(pr_vector_t* vector, int index){
    assert(vector);
    assert(index>=0 && index < vector->d_capacity);
    pr_vector_link_t* link = (pr_vector_link_t*)(vector->d_list_p + index );
    void* value = link->d_value_p;
    link->d_value_p = NULL;
    return value;
}

int pr_vector_capacity(pr_vector_t* vector){
    assert(vector);
    return vector->d_capacity;
}

