//
// Created by PCHEN on 2020-03-17.
//

#include <pr_thread_attr.h>
#include <pr_types.h>
#include <pr_mem.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
struct pr_thread_attr_s{
   int d_stackSize;
   pr_bool_t d_isDetach;
   pr_bool_t d_isCreateSuspended;
};

////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_STACK_SIZE 0

////////////////////////////////////////////////////////////////////////////////
pr_thread_attr_t* pr_thread_attr_new(){
    pr_thread_attr_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_stackSize = DEFAULT_STACK_SIZE;
    p->d_isCreateSuspended = ePR_FALSE;
    p->d_isDetach = ePR_FALSE;
    return p;
}

void pr_thread_attr_delete(pr_thread_attr_t** pAttr){
    assert(pAttr);
    assert(*pAttr);
    FREE(*pAttr);
}

void pr_thread_attr_stacksize_set(pr_thread_attr_t* attr, int stackSize){
    assert(attr);
    attr->d_stackSize = stackSize;
}

int pr_thread_attr_stacksize_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_stackSize;
}

pr_bool_t pr_thread_attr_createsuspended_issupport(pr_thread_attr_t* attr){
    return ePR_TRUE;
}

void pr_thread_attr_createsuspended_set(pr_thread_attr_t* attr, pr_bool_t trueOrFalse){
    assert(attr);
    attr->d_isCreateSuspended = trueOrFalse;
}

pr_bool_t pr_thread_attr_createsuspended_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_isCreateSuspended;
}

pr_bool_t pr_thread_attr_detach_issupport(pr_thread_attr_t* attr){
    return ePR_TRUE;
}

void pr_thread_attr_detach_set(pr_thread_attr_t* attr, pr_bool_t trueOrFalse){
    assert(attr);
    attr->d_isDetach = trueOrFalse;
}

pr_bool_t pr_thread_attr_detach_get(pr_thread_attr_t* attr){
    assert(attr);
    return attr->d_isDetach;
}