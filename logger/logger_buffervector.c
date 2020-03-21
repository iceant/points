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
    link_t * d_freelist_p;
};

struct logger_buffervectoriter_s{
    link_t** d_link_p;
};

////////////////////////////////////////////////////////////////////////////////
logger_buffervector_t* logger_buffervector_new(){
    logger_buffervector_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_freelist_p = NULL;
    return p;
}

void logger_buffervector_delete(logger_buffervector_t** pVector, void (*destructor)(logger_buffer_t*, void* args), void* args){
    assert(pVector);
    assert(*pVector);
    logger_buffervector_t* vector = *pVector;
    if(!destructor) {
        while (vector->d_freelist_p) {
            link_t *p = vector->d_freelist_p;
            vector->d_freelist_p = p->d_next_p;
            FREE(p);
        }
    }else{
        while (vector->d_freelist_p) {
            link_t *p = vector->d_freelist_p;
            vector->d_freelist_p = p->d_next_p;
            destructor(p->d_value_p, args);
            FREE(p);
        }
    }
    FREE(*pVector);
}

void logger_buffervector_add(logger_buffervector_t* vector, logger_buffer_t* buffer){
    assert(vector);
    assert(buffer);
    link_t * p = MALLOC(sizeof(*p));
    assert(p);
    p->d_next_p = vector->d_freelist_p;
    p->d_value_p = buffer;
    vector->d_freelist_p = p;
}

pr_bool_t logger_buffervector_isempty(logger_buffervector_t* vector){
    assert(vector);
    return (vector->d_freelist_p!=NULL)?ePR_TRUE:ePR_FALSE;
}


////////////////////////////////////////////////////////////////////////////////
logger_buffervectoriter_t * logger_buffervectoriter_new(logger_buffervector_t* vector){
    assert(vector);
    logger_buffervectoriter_t* p = MALLOC(sizeof(*p));
    assert(p);
    p->d_link_p = &vector->d_freelist_p;
    return p;
}

void logger_buffervectoriter_delete(logger_buffervectoriter_t** pIter){
    assert(pIter);
    assert(*pIter);
    FREE(*pIter);
}

void logger_buffervectoriter_next(logger_buffervectoriter_t* iter){
    assert(iter);
    *iter->d_link_p = (*iter->d_link_p)->d_next_p;
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
