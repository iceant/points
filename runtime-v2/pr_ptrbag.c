//
// Created by pchen on 12/30/18.
//

#include <pr_ptrbag.h>
#include <pr_mem_util.h>
#include <stdlib.h>
#include <assert.h>

typedef struct pr_ptrbag_link_s {
    void *d_pointer_p;
    pr_ptrbag_release_proc d_releasefn_p;
    struct pr_ptrbag_link_s *d_next_p;
} pr_ptrbag_link_t;

#define PR_PTRBAG_LINK_DELETE(link_p)\
    if((link_p)->d_releasefn_p){\
      (link_p)->d_releasefn_p((link_p)->d_pointer_p);\
    }\
    free((link_p));\
    (link_p)=NULL;

struct pr_ptrbag_s {
    pr_ptrbag_link_t * d_root_p;
};

struct pr_ptrbag_iter_s{
    pr_ptrbag_link_t ** d_link_p;
};

struct pr_ptrbag_manip_s{
    pr_ptrbag_link_t ** d_addrLink_p;
};

//////////////////////////////////////////////////////////////////////////////
//// PtrBagManip

pr_ptrbag_manip_t* pr_ptrbag_manip_new(pr_ptrbag_t* ptrBag){
    assert(ptrBag);
    pr_ptrbag_manip_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_addrLink_p = &(ptrBag->d_root_p);
    return p;
}

void pr_ptrbag_manip_delete(pr_ptrbag_manip_t** ptrBagManip){
    assert(ptrBagManip);
    assert(*ptrBagManip);
    PR_FREE(*ptrBagManip);
}

int pr_ptrbag_manip_hasnext(pr_ptrbag_manip_t* ptrBagManip){
    assert(ptrBagManip);
    return *(ptrBagManip->d_addrLink_p)!=NULL;
}

void* pr_ptrbag_manip_get(pr_ptrbag_manip_t* ptrBagManip){
    assert(ptrBagManip);
    return (*ptrBagManip->d_addrLink_p)->d_pointer_p;
}

void pr_ptrbag_manip_remove(pr_ptrbag_manip_t* ptrBagManip){
    assert(ptrBagManip);
    pr_ptrbag_link_t* tmp = *ptrBagManip->d_addrLink_p;
    *(ptrBagManip->d_addrLink_p) = (*ptrBagManip->d_addrLink_p)->d_next_p;
    PR_PTRBAG_LINK_DELETE(tmp);
}

void pr_ptrbag_manip_next(pr_ptrbag_manip_t* ptrBagManip){
    assert(ptrBagManip);
    ptrBagManip->d_addrLink_p = &(*ptrBagManip->d_addrLink_p)->d_next_p;
}

//////////////////////////////////////////////////////////////////////////////
//// PtrBag

pr_ptrbag_t* pr_ptrbag_new(){
    pr_ptrbag_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_root_p = NULL;
    return p;
}

void pr_ptrbag_delete(pr_ptrbag_t** ptrBag){
    assert(ptrBag);
    assert(*ptrBag);
    pr_ptrbag_manip_t man;
    man.d_addrLink_p = &(*ptrBag)->d_root_p;
    while(pr_ptrbag_manip_hasnext(&man)){
        pr_ptrbag_manip_remove(&man);
    }
    PR_FREE(*ptrBag);
}

void pr_ptrbag_add(pr_ptrbag_t* ptrBag, void *pointer, pr_ptrbag_release_proc releaseFn) {
    assert(ptrBag);
    assert(pointer);

    pr_ptrbag_link_t* p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_pointer_p = pointer;
    p->d_releasefn_p = releaseFn;
    p->d_next_p = ptrBag->d_root_p;
    ptrBag->d_root_p = p;
}

void pr_ptrbag_remove(pr_ptrbag_t* ptrBag, const void* ptr){
    assert(ptrBag);
    pr_ptrbag_manip_t man;
    man.d_addrLink_p = &ptrBag->d_root_p;
    while(pr_ptrbag_manip_hasnext(&man)){
        (pr_ptrbag_manip_get(&man)==ptr)?pr_ptrbag_manip_remove(&man):pr_ptrbag_manip_next(&man);
    }
}

////////////////////////////////////////////////////////////
pr_ptrbag_iter_t* pr_ptrbag_iter_new(pr_ptrbag_t* ptrBag) {
    assert(ptrBag);
    pr_ptrbag_iter_t * p = PR_MALLOC(sizeof(*p));
    assert(p);
    p->d_link_p = &ptrBag->d_root_p;
    return p;
}

int pr_ptrbag_iter_hasnext(pr_ptrbag_iter_t* ptrBagIter) {
    assert(ptrBagIter);
    return *(ptrBagIter->d_link_p)!=NULL;
}

void pr_ptrbag_iter_next(pr_ptrbag_iter_t* ptrBagIter){
    assert(ptrBagIter);
    ptrBagIter->d_link_p = &(*ptrBagIter->d_link_p)->d_next_p;
}

void* pr_ptrbag_iter_get(pr_ptrbag_iter_t* ptrBagIter){
    assert(ptrBagIter);
    return (*ptrBagIter->d_link_p)->d_pointer_p;
}

void pr_ptrbag_iter_delete(pr_ptrbag_iter_t** ptrBagIter) {
    assert(ptrBagIter);
    assert(*ptrBagIter);
    PR_FREE(*ptrBagIter);
}






