//
// Created by PCHEN on 2020-03-18.
//

#include <pr_except.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

pr_except_t TestException = {"Test Failed!"};

void sigHandler(int signum){
    printf("capture signum: %d \n", signum);
    exit(1);
}

int main(int argc, char** argv){

    signal(SIGSEGV, sigHandler);
    signal(SIGTERM, sigHandler);
    signal(SIGABRT, sigHandler);
    signal(SIGFPE, sigHandler);
    signal(SIGILL, sigHandler);
    signal(SIGINT, sigHandler);
    signal(SIGABRT2, sigHandler);

    TRY {
        RAISE(TestException);
    }
    EXCEPT(TestException) {
       fprintf(stderr, "An internal error has occurred from which there is...\n");
       RERAISE;
    }
    ELSE {
        fprintf(stderr, "else block");
    }
    FINALLY {
        fprintf(stderr, "error\n");
    }
    END_TRY;
    return 0;
}