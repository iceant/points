//
// Created by PCHEN on 2020-03-18.
//

#include <pr_except.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
pr_except_frame_t * pr_except_stack_p = NULL;

////////////////////////////////////////////////////////////////////////////////
void pr_except_raise(const pr_except_t* e, const char* file, int line){
    assert(e);
#ifdef WIN32
    pr_except_frame_t *p;
    if(pr_except_index==-1){
        pr_except_init();
    }
    p = TlsGetValue(pr_except_index);
#else
    pr_except_frame_t *p = pr_except_stack_p;
#endif
    if(p==NULL){
        fprintf(stderr, "Uncaught exception ");
        if(e->reason){
            fprintf(stderr, "%s", e->reason);
        }else{
            fprintf(stderr, " at 0x%p", e);
        }
        if(file && line>0){
            fprintf(stderr, " raised at %s:%d\n", file, line);
        }
        fprintf(stderr, "aborting...\n");
        fflush(stderr);
        abort();
    }
    p->exception = e;
    p->file = file;
    p->line = line;

#ifdef WIN32
    pr_except_pop();
#else
    pr_except_stack_p = pr_except_stack_p->prev;
#endif
    longjmp(p->env, ePR_EXCEPT_RAISED);
}

#ifdef WIN32
//_CRTIMP void __cdecl _assert(void*, void*, unsigned);
//#undef assert
//#define assert(e) ((e)||(_assert(#e, __FILE__, __LINE__), 0))

int pr_except_index = -1;

void pr_except_init(){
    BOOL cond;
    pr_except_index = TlsAlloc();
    assert(pr_except_index!=TLS_OUT_OF_INDEXES);
    cond = TlsSetValue(pr_except_index, NULL);
    assert(cond == TRUE);
}

void pr_except_push(pr_except_frame_t * fp){
    BOOL cond;
    fp->prev = TlsGetValue(pr_except_index);
    cond = TlsSetValue(pr_except_index, fp);
    assert(cond==TRUE);
}

void pr_except_pop(){
    BOOL cond;
    pr_except_frame_t * tos = TlsGetValue(pr_except_index);
    cond = TlsSetValue(pr_except_index, tos->prev);
    assert(cond==TRUE);
}
#endif
