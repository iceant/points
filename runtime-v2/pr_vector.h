//
// Created by PCHEN on 2020-03-28.
//

#ifndef INCLUDED_PR_VECTOR_H
#define INCLUDED_PR_VECTOR_H
////////////////////////////////////////////////////////////////////////////////
typedef struct pr_vector_s pr_vector_t;

////////////////////////////////////////////////////////////////////////////////
pr_vector_t* pr_vector_new(int capacity);

void pr_vector_delete(pr_vector_t**, void(*releaseFn)(void*, void* args), void* args);

void pr_vector_set(pr_vector_t*, int index, void*);

void* pr_vector_get(pr_vector_t*, int index);

void* pr_vector_remove(pr_vector_t*, int index);

int pr_vector_capacity(pr_vector_t*);

#endif //INCLUDED_PR_VECTOR_H
