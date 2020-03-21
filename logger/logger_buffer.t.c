//
// Created by PCHEN on 2020-03-20.
//

#include <logger_buffer.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void showBuffer(logger_buffer_t* buffer){
    printf("buffer:\n");
    printf("\tcontent:[%s]\n", logger_buffer_get(buffer));
    printf("\tcapacity:%d\n", logger_buffer_capacity(buffer));
    printf("\tused:%d\n", logger_buffer_used(buffer));
    printf("\tavail:%d\n", logger_buffer_avail(buffer));
}

int main(int argc, char** argv){

    const char* content = "Hello, Chen Peng!\n";
    int size = strlen(content);

    logger_buffer_t* buffer = logger_buffer_new(64);
    logger_buffer_append(buffer, content, size);
    logger_buffer_append(buffer, content, size);
    showBuffer(buffer);
    logger_buffer_delete(&buffer);

    return 0;
}

