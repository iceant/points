//
// Created by PCHEN on 2020-03-18.
//

#ifndef INCLUDED_PR_EXCEPT_H
#define INCLUDED_PR_EXCEPT_H

////////////////////////////////////////////////////////////////////////////////
#ifndef INCLUDED_SETJMP_H
#define INCLUDED_SETJMP_H
#include <setjmp.h>
#endif //INCLUDED_SETJMP_H

////////////////////////////////////////////////////////////////////////////////
typedef struct pr_except_s{
   const char* reason;
}pr_except_t;

typedef struct pr_except_frame_s pr_except_frame_t;
struct pr_except_frame_s{
    pr_except_frame_t * prev;
    jmp_buf env;
    const char* file;
    int line;
    const pr_except_t * exception;
};

enum{
    ePR_EXCEPT_ENTERED=0,
    ePR_EXCEPT_RAISED,
    ePR_EXCEPT_HANDLED,
    ePR_EXCEPT_FINALIZED
};

extern pr_except_frame_t * pr_except_stack_p;
extern const pr_except_t pr_AssertFailed;
////////////////////////////////////////////////////////////////////////////////
void pr_except_raise(const pr_except_t* e, const char* file, int line);

#ifdef WIN32
#include <windows.h>
int pr_except_index;
void pr_except_init(void);
void pr_except_push(pr_except_frame_t * fp);
void pr_except_pop(void);
#endif // WIN32

#ifdef WIN32
#define RAISE(e) pr_except_raise(&(e), __FILE__, __LINE__)
#define RERAISE pr_except_raise(exceptFrame.exception, exceptFrame.file, exceptFrame.line)
#define RETURN switch (pr_except_pop(),0) default: return
#define TRY do { \
	volatile int exceptFlag; \
	pr_except_frame_t exceptFrame; \
	if (pr_except_index == -1) \
		pr_except_init(); \
	pr_except_push(&exceptFrame);  \
	exceptFlag = setjmp(exceptFrame.env); \
	if (exceptFlag == ePR_EXCEPT_ENTERED) {
#define EXCEPT(e) \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_pop(); \
	} else if (exceptFrame.exception == &(e)) { \
		exceptFlag = ePR_EXCEPT_HANDLED;
#define ELSE \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_pop(); \
	} else { \
		exceptFlag = ePR_EXCEPT_HANDLED;
#define FINALLY \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_pop(); \
	} { \
		if (exceptFlag == ePR_EXCEPT_ENTERED) \
			exceptFlag = ePR_EXCEPT_FINALIZED;
#define END_TRY \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_pop(); \
		} if (exceptFlag == ePR_EXCEPT_RAISED) RERAISE; \
} while (0)
#else
#define RAISE(e) pr_except_raise(&(e), __FILE__, __LINE__)
#define RERAISE pr_except_raise(exceptFrame.exception, exceptFrame.file, exceptFrame.line)
#define RETURN switch (pr_except_stack_p = pr_except_stack_p->prev,0) default: return
#define TRY do { \
	volatile int exceptFlag; \
	pr_except_frame_t exceptFrame; \
	exceptFrame.prev = pr_except_stack_p; \
	pr_except_stack_p = &exceptFrame;  \
	exceptFlag = setjmp(exceptFrame.env); \
	if (exceptFlag == ePR_EXCEPT_ENTERED) {
#define EXCEPT(e) \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_stack_p = pr_except_stack_p->prev; \
	} else if (exceptFrame.exception == &(e)) { \
		exceptFlag = ePR_EXCEPT_HANDLED;
#define ELSE \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_stack_p = pr_except_stack_p->prev; \
	} else { \
		exceptFlag = ePR_EXCEPT_HANDLED;
#define FINALLY \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_stack_p = pr_except_stack_p->prev; \
	} { \
		if (exceptFlag == ePR_EXCEPT_ENTERED) \
			exceptFlag = ePR_EXCEPT_FINALIZED;
#define END_TRY \
		if (exceptFlag == ePR_EXCEPT_ENTERED) pr_except_stack_p = pr_except_stack_p->prev; \
		} if (exceptFlag == ePR_EXCEPT_RAISED) RERAISE; \
} while (0)
#endif


#endif //INCLUDED_PR_EXCEPT_H
