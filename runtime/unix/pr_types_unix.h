//
// Created by PCHEN on 2020-03-17.
//

#ifndef INCLUDED_PR_TYPES_UNIX_H
#define INCLUDED_PR_TYPES_UNIX_H

#ifndef INCLUDED_PTHREAD_H
#define INCLUDED_PTHREAD_H
#include <pthread.h>
#endif //INCLUDED_PTHREAD_H

typedef pthread_t pr_thread_handle_t;
typedef pthread_t pr_thread_id_t;

#endif //INCLUDED_PR_TYPES_UNIX_H
