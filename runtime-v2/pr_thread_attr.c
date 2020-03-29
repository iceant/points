//
// Created by PCHEN on 2020-03-25.
//

#include <pr_thread_attr.h>
#include <pr_mem_util.h>
#include <pr_types.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_thread_attr_s{
    pr_bool_t d_is_detach;
    pr_bool_t d_is_stacksizeparamreservation;
    int       d_stacksize;

};

////////////////////////////////////////////////////////////////////////////////
pr_thread_attr_t* pr_thread_attr_new(){
    pr_thread_attr_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_is_detach=ePR_FALSE;
    p->d_stacksize = 0;
    p->d_is_stacksizeparamreservation = ePR_FALSE;
    return p;
}

void pr_thread_attr_delete(pr_thread_attr_t** pAttr){
    assert(pAttr);
    assert(*pAttr);

    PR_FREE(*pAttr);
}

void pr_thread_attr_stacksize_set(pr_thread_attr_t* attr, int size){
    assert(attr);
    assert(size>=0);
    attr->d_stacksize = size;
}

int pr_thread_attr_stacksize_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_stacksize;
}

void pr_thread_attr_detach_set(pr_thread_attr_t* attr, pr_bool_t isDetach){
    assert(attr);
    attr->d_is_detach = isDetach;
}

pr_bool_t pr_thread_attr_detach_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_is_detach;
}

void pr_thread_attr_stacksizeparamisreservation_set(pr_thread_attr_t* attr, pr_bool_t value){
    assert(attr);
    attr->d_is_stacksizeparamreservation = value;
}

pr_bool_t pr_thread_attr_stacksizeparamisreservation_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_is_stacksizeparamreservation;
}
