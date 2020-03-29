//
// Created by PCHEN on 2020-03-25.
//

#include <pr_mutex.h>
#include <pr_types.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static int s_count=0;
static pr_bool_t s_stop = ePR_FALSE;

void threadFn(void* args){
   pr_mutex_t* mutex = (pr_mutex_t*)args;
   while(!s_stop) {
      pr_mutex_lock(mutex);
      s_count += 1;
      printf("[%06ld] %d\n", pthread_self(), s_count);
      pr_mutex_unlock(mutex);
   }
   pthread_exit(NULL);
}

int main(int argc, char** argv){
   pr_mutex_t * mutex_p = pr_mutex_new();
   pthread_t thread1;
   pthread_t thread2;
   pthread_create(&thread1, NULL, threadFn, mutex_p);
   pthread_create(&thread2, NULL, threadFn, mutex_p);

   sleep(10);// sleep 10 seconds
   s_stop = ePR_TRUE;

   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);

   pr_mutex_delete(&mutex_p);
}