//
// Created by PCHEN on 2020-03-23.
//
#include <pr_array.h>
#include <pr_types.h>
#include <pr_mem_util.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_array_s{
    int             d_length;
    int             d_size;
    char*           d_array_p;
};

////////////////////////////////////////////////////////////////////////////////
static void pr_array_init(pr_array_t* array, int length, int size, void* ary){
    assert(array);
    assert(ary && length>0 || length==0 && ary==NULL);
    assert(size>0);
    array->d_length = length;
    array->d_size = size;
    if(length>0){
        array->d_array_p = ary;
    }else{
        array->d_array_p = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
pr_array_t* pr_array_new(int length, int size){
    pr_array_t * p = PR_MALLOC(sizeof(*p));
    assert(p);
    if(length>0){
        void* ary = calloc(length, size);
        assert(ary);
        pr_array_init(p, length, size, ary);
    }else{
        pr_array_init(p, length, size, NULL);
    }
    return p;
}

void pr_array_delete(pr_array_t** pArray){
    assert(pArray);
    assert(*pArray);
    pr_array_t* array = *pArray;
    PR_FREE(array->d_array_p);
    PR_FREE(*pArray);
}

void* pr_array_put(pr_array_t* array, int idx, void* elem){
    assert(array);
    assert(idx>=0 && idx< array->d_length);
    assert(elem);
    memcpy(array->d_array_p + idx*array->d_size, elem, array->d_size);
    return elem;
}

void pr_array_resize(pr_array_t* array, int length){
    assert(array);
    assert(length>=0);
    if(length==0){
        PR_FREE(array->d_array_p);
    }else if(array->d_length==0){
        array->d_array_p = PR_MALLOC(sizeof(length * array->d_size));
        assert(array->d_array_p);
    }else{
        array->d_array_p = realloc(array->d_array_p, length * array->d_size);
        assert(array->d_array_p);
    }
    array->d_length = length;
}

pr_array_t* pr_array_copy(pr_array_t* array, int length){
    pr_array_t* copy;
    assert(array);
    assert(length>0);
    copy = pr_array_new(length, array->d_size);
    if(copy->d_length>=array->d_length && array->d_length>0){
        memcpy(copy->d_array_p, array->d_array_p, array->d_length * array->d_size);
    }else if(array->d_length>copy->d_length && copy->d_length>0){
        memcpy(copy->d_array_p, array->d_array_p, copy->d_length * array->d_size);
    }
    return copy;
}

int pr_array_length(pr_array_t* array){
    assert(array);
    return array->d_length;
}

int pr_array_size(pr_array_t* array){
    assert(array);
    return array->d_size;
}

void* pr_array_get(pr_array_t* array, int idx){
    assert(array);
    assert(idx>=0 && idx<array->d_length);
    return array->d_array_p + idx * array->d_size;
}
