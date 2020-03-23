//
// Created by PCHEN on 2020-03-20.
//
#include <logger_buffervector.h>
#include <logger_buffer.h>
#include <pr_mem.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct link_s{
    struct link_s * d_next_p;
    logger_buffer_t * d_value_p;
}link_t;

struct logger_buffervector_s{
    link_t   d_head;
    link_t * d_tail_p;
    int      d_size;
};

struct logger_buffervectoriter_s{
    link_t** d_link_p;
};

////////////////////////////////////////////////////////////////////////////////
logger_buffervector_t* logger_buffervector_new(){
    logger_buffervector_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_head.d_next_p = NULL;
    p->d_head.d_value_p = NULL;
    p->d_tail_p = &p->d_head;
    p->d_size = 0;
    return p;
}

void logger_buffervector_delete(logger_buffervector_t** pVector, void (*destructor)(logger_buffer_t*, void* args), void* args){
    assert(pVector);
    assert(*pVector);
    logger_buffervector_t* vector = *pVector;
    if(!destructor) {
        while (vector->d_head.d_next_p) {
            link_t *p = vector->d_head.d_next_p;
            vector->d_head.d_next_p = p->d_next_p;
            FREE(p);
        }
    }else{
        while (vector->d_head.d_next_p) {
            link_t *p = vector->d_head.d_next_p;
            vector->d_head.d_next_p = p->d_next_p;
            destructor(p->d_value_p, args);
            FREE(p);
        }
    }
    vector->d_tail_p = &vector->d_head;
    FREE(*pVector);
}

void logger_buffervector_add(logger_buffervector_t* vector, logger_buffer_t* buffer){
    assert(vector);
    assert(buffer);
    link_t * p = MALLOC(sizeof(*p));
    assert(p);
    p->d_value_p = buffer;
    p->d_next_p = NULL;
    vector->d_tail_p->d_next_p = p;
    vector->d_tail_p = p;
    vector->d_size+=1;
}

pr_bool_t logger_buffervector_isempty(logger_buffervector_t* vector){
    assert(vector);
    return (vector->d_size==0)?ePR_TRUE:ePR_FALSE;
}

void logger_buffervector_pop(logger_buffervector_t* vector, logger_buffer_t** buffer){
   assert(vector);
   assert(buffer);
   link_t * p = vector->d_head.d_next_p;
   vector->d_head.d_next_p = p->d_next_p;
   *buffer = p->d_value_p;
   vector->d_size-=1;
   if(vector->d_size==0){
       vector->d_tail_p = &vector->d_head;
   }
   FREE(p);
}

void logger_buffervector_clear(logger_buffervector_t* vector, void (*releaseFn)(logger_buffer_t*, void*), void* args){
    assert(vector);
    link_t* p=NULL;
    if(releaseFn) {
        while (vector->d_head.d_next_p) {
            p = vector->d_head.d_next_p;
            vector->d_head.d_next_p = p->d_next_p;
            releaseFn(p->d_value_p, args);
            FREE(p);
        }
    }else{
        while(vector->d_head.d_next_p){
            p = vector->d_head.d_next_p;
            vector->d_head.d_next_p = p->d_next_p;
            FREE(p);
        }
    }
    vector->d_tail_p = &vector->d_head;
    vector->d_size = 0;
}
////////////////////////////////////////////////////////////////////////////////
logger_buffervectoriter_t * logger_buffervectoriter_new(logger_buffervector_t* vector){
    assert(vector);
    logger_buffervectoriter_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_link_p = &vector->d_head.d_next_p;
    return p;
}

void logger_buffervectoriter_delete(logger_buffervectoriter_t** pIter){
    assert(pIter);
    assert(*pIter);
    FREE(*pIter);
}

void logger_buffervectoriter_next(logger_buffervectoriter_t* iter){
    assert(iter);
    iter->d_link_p = &(*iter->d_link_p)->d_next_p;
}

pr_bool_t logger_buffervectoriter_hasmore(logger_buffervectoriter_t* iter){
    assert(iter);
    return (*iter->d_link_p)!=NULL;
}

logger_buffer_t* logger_buffervectoriter_get(logger_buffervectoriter_t* iter){
    assert(iter);
    if(*iter->d_link_p==NULL) return NULL;
    return (*iter->d_link_p)->d_value_p;
}

void logger_buffervectoriter_remove(logger_buffervectoriter_t* iter){
    assert(iter);
    link_t* p = *iter->d_link_p;
    assert(p);
    *iter->d_link_p = p->d_next_p;
    FREE(p);
}
