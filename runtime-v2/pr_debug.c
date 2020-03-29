//
// Created by PCHEN on 2020-03-29.
//

#include <pr_debug.h>
#include <stdarg.h>
#include <pr_thread.h>
#include <pr_mutex.h>

void pr_debug_fn(const char* file, int line, const char* fmt, ...){
    int ret = 0;
    int contentSize = 256;
    char* content = NULL;
    va_list ap;
    content = malloc(contentSize * sizeof(char));
    assert(content);
    while(1){
        va_start(ap, fmt);
        ret = vsnprintf(content, contentSize, fmt, ap);
        va_end(ap);
        if(ret > -1 && ret < contentSize){
            break;
        }
        contentSize*=2; // expend double size
        content = realloc(content, contentSize * sizeof(char));
        assert(content);
    }

    fprintf(stderr, "[%06ld] %s - %s:%d\n", pr_thread_self(), content, file, line);
    free(content);
    content = NULL;
}

