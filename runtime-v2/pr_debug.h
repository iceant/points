//
// Created by PCHEN on 2020-03-29.
//

#ifndef INCLUDED_PR_DEBUG_H
#define INCLUDED_PR_DEBUG_H

#ifndef INCLUDED_STDLIB_H
#define INCLUDED_STDLIB_H
#include <stdlib.h>
#endif //INCLUDED_STDLIB_H

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif //INCLUDED_STDIO_H

#ifndef INCLUDED_ASSERT_H
#define INCLUDED_ASSERT_H
#include <assert.h>
#endif //INCLUDED_ASSERT_H

////////////////////////////////////////////////////////////////////////////////
void pr_debug_fn(const char* file, int line, const char* fmt, ...);

////////////////////////////////////////////////////////////////////////////////
#define PR_DEBUG(fmt, args...) pr_debug_fn(__FILE__, __LINE__, (fmt), ##args)

#endif //INCLUDED_PR_DEBUG_H
