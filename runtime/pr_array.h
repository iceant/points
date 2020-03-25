//
// Created by PCHEN on 2020-03-23.
//

#ifndef INCLUDED_PR_ARRAY_H
#define INCLUDED_PR_ARRAY_H
////////////////////////////////////////////////////////////////////////////////
typedef struct pr_array_s pr_array_t;

////////////////////////////////////////////////////////////////////////////////
pr_array_t* pr_array_new(int numOfElements, int sizeOfElements);

void pr_array_delete(pr_array_t**);

void* pr_array_put(pr_array_t*, int idx, void* elem);

void pr_array_resize(pr_array_t*, int numOfElements);

pr_array_t* pr_array_copy(pr_array_t*, int numOfElements);

int pr_array_length(pr_array_t*);
    // return the capacity of numOfElements in array

int pr_array_size(pr_array_t*);
    // return the element size in array

void* pr_array_get(pr_array_t*, int idx);

#endif //INCLUDED_PR_ARRAY_H
