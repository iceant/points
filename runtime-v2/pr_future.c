//
// Created by PCHEN on 2020-03-28.
//

#include <pr_future.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_thread.h>
#include <pr_mem_util.h>
#include <pr_types.h>


////////////////////////////////////////////////////////////////////////////////
struct pr_future_s{
    pr_mutex_t* d_lock_p;
    pr_condition_t* d_cond_p;
    pr_thread_t* d_thread_p;

    pr_future_function d_function;
    void* d_args;
    void* d_result;

    pr_bool_t d_executed;
};

////////////////////////////////////////////////////////////////////////////////
void pr_future_threadFn(void* args){
   assert(args);
   pr_future_t* future = (pr_future_t*)args;
   if(future->d_function){
      future->d_result = future->d_function(future->d_args);
   }
   pr_mutex_lock(future->d_lock_p);
   future->d_executed = ePR_TRUE;
   pr_condition_notifyall(future->d_cond_p);
   pr_mutex_unlock(future->d_lock_p);
}

////////////////////////////////////////////////////////////////////////////////
pr_future_t* pr_future_new(pr_future_function function, void* args){
   assert(function);
   pr_future_t* p = PR_MALLOC(sizeof(*p));
   assert(p);

   p->d_executed = ePR_FALSE;
   p->d_function = function;
   p->d_args = args;
   p->d_result = NULL;

   p->d_lock_p = pr_mutex_new();
   assert(p->d_lock_p);
   p->d_cond_p = pr_condition_new(p->d_lock_p);
   assert(p->d_cond_p);

   p->d_thread_p = pr_thread_new(NULL, pr_future_threadFn, p);

   return p;
}

void pr_future_delete(pr_future_t** pFuture){
   assert(pFuture);
   assert(*pFuture);

   pr_future_t* future = *pFuture;

   pr_mutex_lock(future->d_lock_p);
   if(future->d_thread_p){
      while(!future->d_executed){
         pr_condition_wait(future->d_cond_p);
      }
      pr_thread_delete(&future->d_thread_p);
   }
   pr_mutex_unlock(future->d_lock_p);

   pr_condition_delete(&future->d_cond_p);
   pr_mutex_delete(&future->d_lock_p);
   PR_FREE(*pFuture);
}

void* pr_future_get(pr_future_t* future){
   assert(future);

   pr_mutex_lock(future->d_lock_p);
   while(!future->d_executed){
      pr_condition_wait(future->d_cond_p);
   }
   pr_mutex_unlock(future->d_lock_p);

   return future->d_result;
}

