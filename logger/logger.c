//
// Created by PCHEN on 2020-03-20.
//

#include <logger.h>
#include <logger_buffer.h>
#include <logger_buffervector.h>
#include <pr_threadlocal.h>
#include <pr_types.h>
#include <pr_mem.h>
#include <pr_mutex.h>
#include <pr_condition.h>
#include <pr_thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
struct logger_s{
    FILE*                       d_file_p;

    pr_mutex_t *                d_lock_p;
    pr_condition_t *            d_cond_p;
    pr_condition_t *            d_stopCond_p;

    pr_thread_t*                d_thread_p;
    pr_threadlocal_t*           d_threadId_p;

    logger_buffer_t *           d_current_p;
    logger_buffer_t *           d_next_p;

    logger_buffervector_t *     d_buffers_p;

    void*                       d_stopArgs;
    logger_stopCallback         d_stopCallback;
    int                         d_status;
    logger_level_t              d_level;
};

////////////////////////////////////////////////////////////////////////////////
#define LOGGER_LEVEL(level) (((level)==eLOGGER_LEVEL_FATAL)?"FATAL":(((level)==eLOGGER_LEVEL_ERROR)?"ERROR":((level==eLOGGER_LEVEL_INFO)?"INFO":((level==eLOGGER_LEVEL_DEBUG)?"DEBUG":"INFO"))))

#define BUFFER_SIZE 4096
#define FLUSH_INTERVAL 3

#define STATUS_RUNNING 1
#define STATUS_STOPPED -1
#define STATUS_STOPPING -2
#define STATUS_NA 0

////////////////////////////////////////////////////////////////////////////////
// static functions

//static __thread pr_thread_id_t s_tls_threadId;

static inline void getTimestamp(char* currentTime){
    time_t rawtime;
    struct timeval now;
    time(&rawtime);
    struct tm time = *gmtime(&rawtime);
    gettimeofday(&now, NULL);

    sprintf(currentTime, "%04d-%02d-%02d %02d:%02d:%02d.%06ld",
            time.tm_year+1900, time.tm_mon+1, time.tm_mday,
            time.tm_hour, time.tm_min, time.tm_sec
            , now.tv_usec);
}

static inline pr_thread_id_t getCurrentThreadId(logger_t* logger){
    pr_thread_id_t id = (pr_thread_id_t)pr_threadlocal_get(logger->d_threadId_p);
    if(id==0){
       id = pr_thread_self();
       pr_threadlocal_set(logger->d_threadId_p, (void*)id);
    }
    return id;
}

static inline void flushBuffersToFile(logger_t* logger, logger_buffervector_t* vector){
    assert(logger);
    assert(vector);
    logger_buffervectoriter_t* iter = logger_buffervectoriter_new(vector);
    while(logger_buffervectoriter_hasmore(iter)){
        logger_buffer_t* buffer = logger_buffervectoriter_get(iter);
        fwrite(logger_buffer_get(buffer), sizeof(char), logger_buffer_used(buffer), logger->d_file_p);
//        logger_buffer_delete(&buffer);
//        logger_buffervectoriter_remove(iter);
        logger_buffervectoriter_next(iter);
    }
    logger_buffervectoriter_delete(&iter);
    fflush(logger->d_file_p);
}

static void bufferRelease(logger_buffer_t* buffer, void* args){
    assert(buffer);
    logger_buffer_delete(&buffer);
}

static void logger_threadFn(void* args){
    assert(args);
    logger_t* logger = (logger_t*)args;

    logger_buffer_t* newBuffer1;
    logger_buffer_t* newBuffer2;

    newBuffer1 = logger_buffer_new(BUFFER_SIZE);
    newBuffer2 = logger_buffer_new(BUFFER_SIZE);
    logger_buffervector_t * buffersToWrite = logger_buffervector_new();

    while(logger->d_status==STATUS_RUNNING){
        pr_mutex_lock(logger->d_lock_p);
        if(logger_buffervector_isempty(logger->d_buffers_p)){
            pr_condition_waitfor(logger->d_cond_p, FLUSH_INTERVAL);
        }
        logger_buffervector_add(logger->d_buffers_p, logger->d_current_p);
        logger->d_current_p = newBuffer1;
        newBuffer1 = NULL;

        LOGGER_BUFFERVECTOR_SWAP(logger->d_buffers_p, buffersToWrite);

        if(logger->d_next_p==NULL){
           logger->d_next_p = newBuffer2;
           newBuffer2 = NULL;
        }
        pr_mutex_unlock(logger->d_lock_p);

        // flush to file
        flushBuffersToFile(logger, buffersToWrite);

        // refill newBuffer1, newBuffer2

//        newBuffer1 = logger_buffer_new(BUFFER_SIZE);
//        if(newBuffer2==NULL){
//            newBuffer2 = logger_buffer_new(BUFFER_SIZE);
//        }
        logger_buffervector_pop(buffersToWrite, &newBuffer1);
        logger_buffer_reset(newBuffer1);
        if(newBuffer2==NULL){
            logger_buffervector_pop(buffersToWrite, &newBuffer2);
            logger_buffer_reset(newBuffer2);
        }
        logger_buffervector_clear(buffersToWrite, bufferRelease, NULL);
    }

    pr_mutex_lock(logger->d_lock_p);
    if(logger->d_current_p!=NULL){
        logger_buffervector_add(logger->d_buffers_p, logger->d_current_p);
        logger->d_current_p= NULL;
    }
    LOGGER_BUFFERVECTOR_SWAP(logger->d_buffers_p, buffersToWrite);
    pr_mutex_unlock(logger->d_lock_p);

    flushBuffersToFile(logger, buffersToWrite);
    if(newBuffer1!=NULL){
        logger_buffer_delete(&newBuffer1);
    }
    if(newBuffer2!=NULL){
        logger_buffer_delete(&newBuffer2);
    }
    if(buffersToWrite!=NULL){
        logger_buffervector_delete(&buffersToWrite, bufferRelease, NULL);
    }

    if(logger->d_stopCallback){
        logger->d_stopCallback(logger, logger->d_stopArgs);
    }

    pr_mutex_lock(logger->d_lock_p);
    logger->d_status = STATUS_STOPPED;
    pr_condition_notify(logger->d_stopCond_p);
    pr_mutex_unlock(logger->d_lock_p);

    pr_thread_exit(0);
}

static inline
void logger_append(logger_t* logger, const char* logline, int len){
    assert(logger);
    assert(logline);
    pr_mutex_lock(logger->d_lock_p);
    if(logger_buffer_avail(logger->d_current_p)>=len){
        logger_buffer_append(logger->d_current_p, logline, len);
    }else{
        logger_buffervector_add(logger->d_buffers_p, logger->d_current_p);
        logger->d_current_p = NULL;
        if(logger->d_next_p!=NULL){
            logger->d_current_p = logger->d_next_p;
            logger->d_next_p = NULL;
        }else{
            logger->d_current_p = logger_buffer_new(BUFFER_SIZE);
        }
        logger_buffer_append(logger->d_current_p, logline, len);
        pr_condition_notify(logger->d_cond_p);// notify buffers need to be flushed.
    }
    pr_mutex_unlock(logger->d_lock_p);
}

////////////////////////////////////////////////////////////////////////////////
logger_t * logger_new(const char* file, logger_level_t level){
    assert(file);
    logger_t * logger = MALLOC(sizeof(*logger));
    assert(logger);
    logger->d_file_p = fopen(file, "a+");
    assert(logger->d_file_p);
    logger->d_lock_p = pr_mutex_new();
    assert(logger->d_lock_p);
    logger->d_cond_p = pr_condition_new(logger->d_lock_p);
    assert(logger->d_cond_p);
    logger->d_stopCond_p = pr_condition_new(logger->d_lock_p);
    assert(logger->d_stopCond_p);

    logger->d_buffers_p = logger_buffervector_new();

    logger->d_current_p = logger_buffer_new(BUFFER_SIZE);
    assert(logger->d_current_p);
    logger->d_next_p = logger_buffer_new(BUFFER_SIZE);
    assert(logger->d_next_p);

    logger->d_thread_p = NULL;
    logger->d_threadId_p = pr_threadlocal_new(NULL);

    logger->d_status = STATUS_NA;
    logger->d_level = level;
    logger->d_stopArgs = NULL;
    logger->d_stopCallback = NULL;
    return logger;
}

void logger_delete(logger_t** pLogger){
   assert(pLogger);
   assert(*pLogger);
   logger_t* logger = *pLogger;

   pr_mutex_lock(logger->d_lock_p);
   if(logger->d_status!=STATUS_STOPPED){
       pr_condition_wait(logger->d_stopCond_p);
   }

   if(logger->d_thread_p){
       pr_thread_delete(&logger->d_thread_p);
   }
   pr_threadlocal_delete(&logger->d_threadId_p);

   if(logger->d_current_p) {
       logger_buffer_delete(&logger->d_current_p);
   }
   if(logger->d_next_p){
       logger_buffer_delete(&logger->d_next_p);
   }
   logger_buffervector_delete(&logger->d_buffers_p, bufferRelease, NULL);
   fclose(logger->d_file_p);
   logger->d_status = STATUS_NA;
   pr_mutex_unlock(logger->d_lock_p);

   pr_condition_delete(&logger->d_cond_p);
   pr_mutex_delete(&logger->d_lock_p);

   FREE(*pLogger);
}

void logger_start(logger_t* logger, void(*startCallback)(logger_t*, void* args), void* args){
    if(logger->d_status==STATUS_RUNNING) return;
    pr_mutex_lock(logger->d_lock_p);
    logger->d_status = STATUS_RUNNING;
    logger->d_thread_p = pr_thread_new(NULL, logger_threadFn, logger);
    pr_mutex_unlock(logger->d_lock_p);
    if(startCallback){
        startCallback(logger, args);
    }
}

void logger_stop(logger_t* logger, logger_stopCallback callback, void* args){
   assert(logger);
   if(logger->d_status!=STATUS_RUNNING)return;
   pr_mutex_lock(logger->d_lock_p);
   logger->d_stopCallback = callback;
   logger->d_stopArgs = args;
   logger->d_status = STATUS_STOPPING;
   pr_mutex_unlock(logger->d_lock_p);
}

pr_bool_t logger_isrunning(logger_t* logger){
    assert(logger);
    return (logger->d_status==STATUS_RUNNING)?ePR_TRUE:ePR_FALSE;
}

pr_bool_t logger_isstopped(logger_t* logger){
    assert(logger);
    return (logger->d_status==STATUS_STOPPED)?ePR_TRUE:ePR_FALSE;
}

void logger_printf(logger_t* logger, logger_level_t level, const char* file, const int line, const char* fmt, ...){
    if(logger->d_level<level) return;
    // 1. get timestamp
    char timestamp[84];
    getTimestamp(timestamp);

    // 2. get thread id
    pr_thread_id_t threadId = getCurrentThreadId(logger);

    // 3. get content
    int ret;
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

    int linelogSize = 256;
    char* linelog = NULL;
    linelog = malloc(linelogSize * sizeof(char));
    while(1){
        ret = sprintf(linelog, "%s %09lu %s %s - %s:%d\n", timestamp, threadId, LOGGER_LEVEL(level), content, file, line);
        if(ret>-1 && ret<linelogSize){
            break;
        }
        linelogSize*=2;
        linelog = realloc(linelog, linelogSize * sizeof(char));
    }
    FREE(content);
    logger_append(logger, linelog, ret);
    FREE(linelog);
}
