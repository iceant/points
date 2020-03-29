//
// Created by PCHEN on 2020-03-26.
//

#ifdef WIN32
    #ifdef WITH_PTHREAD
        #include "unix/pr_thread_local.c"
    #else
        #include "windows/pr_thread_local.c"
    #endif //WITH_PTHREAD
#else // WIN32
    #include "unix/pr_thread_local.c"
#endif