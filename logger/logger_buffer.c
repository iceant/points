//
// Created by PCHEN on 2020-03-20.
//
#include <logger_buffer.h>
#include <pr_types.h>
#include <pr_mem.h>
#include <stdlib.h>
#include <assert.h>


////////////////////////////////////////////////////////////////////////////////
struct logger_buffer_s{
    char* d_data_p;
    int   d_capacity;
    int   d_used;
};

////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_CHUNK_SIZE 10

////////////////////////////////////////////////////////////////////////////////
logger_buffer_t * logger_buffer_new(int capacity){
    assert(capacity>0);
    logger_buffer_t* buffer = MALLOC(sizeof(*buffer));
    assert(buffer);
    buffer->d_capacity = capacity;
    buffer->d_data_p = MALLOC(sizeof(char)*capacity);
    buffer->d_used = 0;
    memset(buffer->d_data_p, 0, capacity);
    return buffer;
}

void logger_buffer_delete(logger_buffer_t** pBuffer){
    assert(pBuffer);
    assert(*pBuffer);
    FREE((*pBuffer)->d_data_p);
    FREE(*pBuffer);
}

void logger_buffer_append(logger_buffer_t* buffer, const char* content ,int length){
    assert(buffer);
    assert(content);
    assert(length>0);
    assert((buffer->d_capacity-buffer->d_used)>=length);
    memcpy(buffer->d_data_p+buffer->d_used, content, length);
    buffer->d_used += length;
}

const char* logger_buffer_get(logger_buffer_t* buffer){
    assert(buffer);
    return buffer->d_data_p;
}

int logger_buffer_capacity(logger_buffer_t* buffer){
    assert(buffer);
    return buffer->d_capacity;
}

int logger_buffer_used(logger_buffer_t* buffer){
    assert(buffer);
    return buffer->d_used;
}

int logger_buffer_avail(logger_buffer_t* buffer){
    assert(buffer);
    return buffer->d_capacity-buffer->d_used;
}

void logger_buffer_reset(logger_buffer_t* buffer){
    assert(buffer);
    memset(buffer->d_data_p, 0, buffer->d_capacity);
    buffer->d_used = 0;
}

