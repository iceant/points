//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_MEM_H
#define INCLUDED_PR_MEM_H

#ifndef INCLUDED_STDLIB_H
#define INCLUDED_STDLIB_H
#include <stdlib.h>
#endif //INCLUDED_STDLIB_H


////////////////////////////////////////////////////////////////////////////////
union pr_align_u {
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

////////////////////////////////////////////////////////////////////////////////
#define ALIGNMENT (sizeof(union pr_align_u))
#define ALIGN(nbytes)\
    (((nbytes) + ALIGNMENT -1 )/ALIGNMENT*ALIGNMENT)

#define MALLOC(nbytes) malloc(ALIGN((nbytes)));
#define FREE(ptr) free((ptr)); (ptr) = NULL

#endif //INCLUDED_PR_MEM_H
