//
// Created by PCHEN on 2020-03-20.
//
#include <logger_buffervector.h>
#include <logger_buffer.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void bufferRelease(logger_buffer_t* buffer, void* args){
    assert(buffer);
    assert(args);
    logger_buffer_allocator_t* bufferAllocator = (logger_buffer_allocator_t*)args;
    logger_buffer_delete(&buffer, bufferAllocator);
}

int main(int argc, char** argv){

    logger_buffer_allocator_t* bufferAllocator_p = logger_buffer_allocator_new(64);

    logger_buffervector_t* vector = logger_buffervector_new();

    for(int i=0; i<10; i++){
        logger_buffer_t* buffer = logger_buffer_new(bufferAllocator_p);
        logger_buffervector_add(vector, buffer);
    }

    logger_buffervector_delete(&vector, bufferRelease, bufferAllocator_p);

    logger_buffer_allocator_delete(&bufferAllocator_p);
}

