//
// Created by PCHEN on 2020-03-25.
//

#include <pr_mutex.h>

#ifdef WIN32
    #ifdef WITH_PTHREAD
        #include "unix/pr_mutex.c"
    #else
        #include "windows/pr_mutex.c"
    #endif //USE_PTHREAD
#else // WIN32
    #include "unix/pr_mutex.c"
#endif

