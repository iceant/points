//
// Created by PCHEN on 2020-03-25.
//

#ifdef WIN32
    #ifdef WITH_PTHREAD
        #include "unix/pr_thread.c"
    #else
        #include "windows/pr_thread.c"
    #endif //WITH_PTHREAD
#else // WIN32
    #include "unix/pr_thread.c"
#endif // WIN32
