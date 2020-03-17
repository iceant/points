//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_TYPES_H
#define INCLUDED_PR_TYPES_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif //INCLUDED_STDINT_H

typedef enum pr_bool_e{
    ePR_FALSE=0,
    ePR_TRUE
}pr_bool_t;

#ifdef USE_PTHREAD
#include "unix/pr_types_unix.h"
#else
#include "windows/pr_types_win.h"
#endif

#endif //INCLUDED_PR_TYPES_H
