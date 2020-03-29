//
// Created by PCHEN on 2020-03-25.
//

#ifndef INCLUDE_PR_TYPES_H
#define INCLUDE_PR_TYPES_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif //INCLUDED_STDINT_H

#ifndef INCLUDED_TIME_H
#define INCLUDED_TIME_H
#include <time.h>
#endif //INCLUDED_TIME_H

#ifdef USE_PTHREAD
    #ifndef INCLUDED_PTHREAD_PR_TYPES_H
    #define INCLUDED_PTHREAD_PR_TYPES_H
    #include <unix/pr_types.h>
    #endif //INCLUDED_PTHREAD_PR_TYPES_H
#else
    #ifndef INCLUDED_WINDOWS_PR_TYPES_H
    #define INCLUDED_WINDOWS_PR_TYPES_H
    #include <windows/pr_types.h>
    #endif // INCLUDED_WINDOWS_PR_TYPES_H
#endif

typedef enum pr_bool_e{
   ePR_FALSE=0,
   ePR_TRUE=1
}pr_bool_t;

typedef unsigned char   pr_byte_t;
typedef void*           pr_handle_t;
typedef int             pr_status_t;
typedef intptr_t        pr_intptr_t;
typedef time_t          pr_time_t;

#define pr_null_t       NULL

#include <pr_errnos.h>

#endif //INCLUDE_PR_TYPES_H
