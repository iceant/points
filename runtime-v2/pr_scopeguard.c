//
// Created by PCHEN on 2020-03-17.
//

#include <pr_scopeguard.h>
#include <pr_mem_util.h>
#include <pr_types.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_scopeguard_link_s{
    struct pr_scopeguard_link_s * d_next_p;
    pr_scopeguard_proc d_proc;
    void* d_args;
}pr_scopeguard_link_t;

struct pr_scopeguard_s{
    pr_scopeguard_link_t * d_head_p;
    pr_bool_t d_isDismissed;
};

////////////////////////////////////////////////////////////////////////////////
pr_scopeguard_t* pr_scopeguard_new(){
    pr_scopeguard_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_head_p = NULL;
    p->d_isDismissed = ePR_FALSE;
    return p;
}

void pr_scopeguard_delete(pr_scopeguard_t** pGuard){
   assert(pGuard);
   assert(*pGuard);
   pr_scopeguard_t* guard = *pGuard;
   if(!guard->d_isDismissed) {
       while (guard->d_head_p) {
           pr_scopeguard_link_t *q = guard->d_head_p;
           guard->d_head_p = q->d_next_p;
           if (q->d_proc) {
               q->d_proc(q->d_args);
           }
           PR_FREE(q);
       }
   }else{
       while(guard->d_head_p){
           pr_scopeguard_link_t * q = guard->d_head_p;
           guard->d_head_p = q->d_next_p;
           PR_FREE(q);
       }
   }
   PR_FREE(*pGuard);
}

void pr_scopeguard_onexit(pr_scopeguard_t* guard, pr_scopeguard_proc proc, void* args){
    assert(guard);
    assert(proc);
    pr_scopeguard_link_t * p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_proc = proc;
    p->d_args = args;
    p->d_next_p = guard->d_head_p;
    guard->d_head_p = p;
}

void pr_scopeguard_dismiss(pr_scopeguard_t* guard){
    assert(guard);
    guard->d_isDismissed = ePR_TRUE;
}

pr_bool_t pr_scopeguard_isdismissed(pr_scopeguard_t* guard){
    assert(guard);
    return guard->d_isDismissed;
}
