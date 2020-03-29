//
// Created by PCHEN on 2020-03-25.
//

#ifdef WIN32
    #ifdef WITH_PTHREAD
        #include "unix/pr_condition.c"
    #else
        #include "windows/pr_condition.c"
    #endif //WITH_PTHREAD
#else
    #include "unix/pr_condition.c"
#endif