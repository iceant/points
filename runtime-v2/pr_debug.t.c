//
// Created by PCHEN on 2020-03-29.
//

#include <pr_debug.h>
#include <assert.h>

int main(int argc, char** argv){
    for(int i=0; i<10; i++){
        PR_DEBUG("%d", i);
    }
}
