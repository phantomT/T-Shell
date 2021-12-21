//
// Created by tbb on 2021/12/18.
//
#include "main.h"

void sigint_handler(int sig){
    printf("Got a INT SIGNAL\n");
    pid_t pid = getpid();
    if(pid != 0)
        kill(-pid, sig);
    exit(0);
}
void sigtstp_handler(int sig){
    printf("Got a STOP SIGNAL\n");
    pid_t pid = getpid();
    if(pid != 0)
        kill(-pid, sig);
    exit(0);
}