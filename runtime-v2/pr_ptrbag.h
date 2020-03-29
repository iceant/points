//
// Created by pchen on 12/30/18.
//

#ifndef INCLUDED_PR_PTRBAG_H
#define INCLUDED_PR_PTRBAG_H

//////////////////////////////////////////////////////////////////////////////
//// TYPES
typedef void (*pr_ptrbag_release_proc)(void*);

typedef struct pr_ptrbag_s pr_ptrbag_t;
typedef struct pr_ptrbag_iter_s pr_ptrbag_iter_t;
typedef struct pr_ptrbag_manip_s pr_ptrbag_manip_t;

//////////////////////////////////////////////////////////////////////////////
//// pr_ptrbag_iter
pr_ptrbag_iter_t* pr_ptrbag_iter_new(pr_ptrbag_t*);
void pr_ptrbag_iter_delete(pr_ptrbag_iter_t**);
int pr_ptrbag_iter_hasnext(pr_ptrbag_iter_t*);
void pr_ptrbag_iter_next(pr_ptrbag_iter_t*);
void* pr_ptrbag_iter_get(pr_ptrbag_iter_t*);

//////////////////////////////////////////////////////////////////////////////
//// pr_ptrbag_manip manipulator for ptrbag
pr_ptrbag_manip_t* pr_ptrbag_manip_new(pr_ptrbag_t*);
void pr_ptrbag_manip_delete(pr_ptrbag_manip_t**);
int pr_ptrbag_manip_hasnext(pr_ptrbag_manip_t *);
void pr_ptrbag_manip_next(pr_ptrbag_manip_t*);
void* pr_ptrbag_manip_get(pr_ptrbag_manip_t*);
void pr_ptrbag_manip_remove(pr_ptrbag_manip_t*);

//////////////////////////////////////////////////////////////////////////////
//// pr_ptrbag_t
pr_ptrbag_t* pr_ptrbag_new();
void pr_ptrbag_delete(pr_ptrbag_t**);

void pr_ptrbag_add(pr_ptrbag_t*, void* pointer, pr_ptrbag_release_proc);
void pr_ptrbag_remove(pr_ptrbag_t*, const void* ptr);

#endif //INCLUDED_PR_PTRBAG_H
