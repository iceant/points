//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDED_PR_MEM_UTIL_H
#define INCLUDED_PR_MEM_UTIL_H

#ifndef INCLUDED_STDLIB_H
#define INCLUDED_STDLIB_H
#include <stdlib.h>
#endif //INCLUDED_STDLIB_H

#ifndef INCLUDED_ASSERT_H
#define INCLUDED_ASSERT_H
#include <assert.h>
#endif //INCLUDED_ASSERT_H

////////////////////////////////////////////////////////////////////////////////
union pr_mem_align_u {
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
#define PR_ALIGNMENT (sizeof(union pr_mem_align_u))
#define PR_ALIGN(nbytes)\
    (((nbytes) + PR_ALIGNMENT -1 )&~(PR_ALIGNMENT -1))

#define PR_MALLOC(size) \
    malloc(PR_ALIGN((size)))

#define PR_NEW(p)\
    (p) = PR_MALLOC(sizeof(*(p))); assert((p))

#define PR_FREE(p) \
    free((p)); (p) = NULL

#endif //INCLUDED_PR_MEM_UTIL_H
