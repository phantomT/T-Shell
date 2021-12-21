//
// Created by tbb on 2021/12/6.
//
#include "main.h"

void exec_cmd(cmd_t _root){
    pid_t pid;
    if(_root->type == CMD_ATOM){
        cmd_atom a_cmd = (cmd_atom)_root;
        node _node = a_cmd->node;
        if(!strcmp(_node->data, "exit") ||
            !strcmp(_node->data, "quit"))
            exit(0);
        else if(!strcmp(_node->data, "cd")) {
            save_history(root);
            cd_cmd(_node->next);
        }else if(!strcmp(_node->data, "about")) {
            printf("This is a simplified version of shell on Linux.\n");
            save_history(root);
        }else if(!strcmp(_node->data, "history")) {
            history_cmd(_node->next);
            save_history(root);
        }else if(!strcmp(_node->data, "can_you_hear_me?")) {
            printf("Hell yeah!\n");
            save_history(root);
        }else if(!strcmp(_node->data, "who_am_i")){
            printf("How could I know\n");
            save_history(root);
        }else{
            switch(pid = fork()){
                case -1: {
                    printf("fork failed.\n");
                    return;
                }
                case 0:{
                    cmd_run(_root);
                    printf("%s: Command Error\n", _node->data);
                    exit(1);
                }
                default:{
                    signal(SIGINT, SIG_IGN);
                    signal(SIGTSTP, SIG_IGN);
                    int status;
                    waitpid(pid, &status, 0);
                    signal(SIGINT, sigint_handler);
                    signal(SIGTSTP, sigtstp_handler);
                    save_history(root);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s\n", strerror(err));
                }
            }
        }
    }else{
        switch(pid = fork()){
            case -1: {
                printf("fork failed.\n");
                return;
            }
            case 0:{
                cmd_run(_root);
                printf("Command Error\n");
                exit(1);
            }
            default:{
                signal(SIGINT, SIG_IGN);
                signal(SIGTSTP, SIG_IGN);
                int status;
                waitpid(pid, &status, 0);
                save_history(root);
                signal(SIGINT, sigint_handler);
                signal(SIGTSTP, sigtstp_handler);
                int err = WEXITSTATUS(status);
                if(err)
                    printf("Error: %s\n", strerror(err));
            }
        }
    }
}

void cmd_run(cmd_t cmd){
    pid_t pid;
    switch(cmd->type){
        case CMD_ATOM:{
            char *arg[10] = {0};
            int i = 0;
            cmd_atom t = (cmd_atom)cmd;
            node _node = t->node;

            if(!strcmp(_node->data, "exit") || !strcmp(_node->data, "quit")){
                printf("do exit\n");
                exit(0);
            }else if(!strcmp(_node->data, "cd")){
                cd_cmd(_node->next);
                break;
            }else if(!strcmp(_node->data, "about")){
                printf("This is a simplified version of shell on Linux.\n");
                break;
            }else if(!strcmp(_node->data, "history")){
                history_cmd(_node->next);
                break;
            }else if(!strcmp(_node->data, "can_you_hear_me")){
                printf("Hell yeah!\n");
                break;
            }else if(!strcmp(_node->data, "who_am_i")){
                printf("How could I know");
                break;
            }else{
                while(_node){
                    arg[i] = _node->data;
                    _node = _node->next;
                    ++i;
                }
                //use cmd already in $PATH, so use execvp() instead of execv()
                if(execvp(arg[0], arg) == -1) {
                    char *curPath = malloc(strlen(pathName)+strlen(arg[0])+2);
                    if(curPath == NULL){
                        printf("execv in curPath malloc failed.\n");
                        exit(1);
                    }
                    strncpy(curPath, pathName, strlen(pathName)+1);
                    strncpy(curPath+ strlen(pathName), "/", 2);
                    strncpy(curPath+ strlen(pathName)+1, arg[0], strlen(arg[0])+1);
                    printf("%s\n", curPath);
                    if(execv(curPath, arg) == -1)
                        fprintf(stderr, "Cannot run command, check your input.\n");
                }
                break;
            }
        }
        case CMD_BACK:{
            cmd_back t = (cmd_back)cmd;
            cmd_t _back = t->back;
            switch (pid = fork()) {
                case -1: {
                    printf("fork failed.\n");
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    freopen("/dev/null", "w", stdout);
                    freopen("/dev/null", "r", stdin);
                    signal(SIGCHLD, SIG_IGN);
                    sigaddset(&sigset, SIGINT);
                    sigprocmask(SIG_SETMASK, &sigset, NULL);
                    cmd_run(_back);
                }
                default:
                    break;
            }
            break;
        }
        case CMD_LIST:{
            cmd_list t = (cmd_list)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;
            switch(pid = fork()){
                case -1: {
                    printf("fork failed.\n");
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    cmd_run(left);
                    printf("List Command Error\n");
                    exit(1);
                }
                default:{
                    int status;
                    waitpid(pid, &status, 0);
                    cmd_run(right);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s\n", strerror(err));
                }
            }
            break;
        }
        case CMD_PIPE:{
            cmd_pipe t = (cmd_pipe)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;
            int pd[2];
            if(pipe(pd) < 0){
                perror("pipe()");
                exit(1);
            }
            switch (pid = fork()) {
                case -1:{
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    close(pd[0]);
                    dup2(pd[1], STDOUT_FILENO);
                    cmd_run(left);
                    if(pd[1] != STDOUT_FILENO)
                        close(pd[1]);
                    break;
                }
                default:{
                    int status;
                    waitpid(pid, &status, 0);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s\n", strerror(err));

                    close(pd[1]);
                    dup2(pd[0], STDIN_FILENO);
                    cmd_run(right);
                    if(pd[0] != STDIN_FILENO)
                        close(pd[0]);
                    break;
                }
            }
            break;
        }
        case CMD_REDI:{
            cmd_redi t = (cmd_redi)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;
            //if(right->type == CMD_ATOM) printf("redi: it's a atom\n");
            //else if(right->type == CMD_REDO) printf("redi: it's a redo\n");
            switch (pid = fork()) {
                case -1:{
                    printf("Redi fork() Error.\n");
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    int fd;
                    cmd_atom inFile;
                    char *File;

                    inFile = (cmd_atom)right;
                    File = inFile->node->data;

                    fd = open(File, O_RDONLY, 0777);
                    if(fd < 0)
                        exit(1);
                    dup2(fd, STDIN_FILENO);
                    cmd_run(left);
                    if(fd != STDIN_FILENO)
                        close(fd);
                    exit(1);
                }
                default:{
                    int status;
                    waitpid(pid, &status, 0);
                    if(right->type != CMD_ATOM)
                        cmd_run(right);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s\n.", strerror(err));
                }
            }
            break;
        }
        case CMD_REDO:{
            cmd_redo t = (cmd_redo)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;
            //if(left->type == CMD_REDI) printf("redo: it's a redi\n");
            //else if(left->type == CMD_ATOM) printf("redo: it's a atom\n");
            switch (pid = fork()) {
                case -1:{
                    printf("Redo fork() Error\n.");
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    int fd;
                    cmd_atom outFile;
                    char *File;

                    outFile = (cmd_atom)right;
                    File = outFile->node->data;

                    fd = open(File, O_WRONLY|O_CREAT|O_TRUNC, 0777);
                    if(fd < 0)
                        exit(1);
                    dup2(fd, STDOUT_FILENO);
                    cmd_run(left);
                    if(fd != STDOUT_FILENO)
                        close(fd);
                    exit(1);
                }
                default:{
                    int status;
                    waitpid(pid, &status, 0);
                    if(right->type != CMD_ATOM)
                        cmd_run(right);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s.\n", strerror(err));
                }
            }
            break;
        }
        case CMD_REDOR:{
            cmd_redo t = (cmd_redo)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;
            switch (pid = fork()) {
                case -1:{
                    printf("Redor fork() Error.\n");
                    perror("fork()");
                    exit(1);
                }
                case 0:{
                    int fd;
                    cmd_atom outFile;
                    char *File;

                    if(right->type == CMD_ATOM){
                        outFile = (cmd_atom)right;
                        File = outFile->node->data;
                    }else if(right->type == CMD_REDO){
                        cmd_redo tmp = (cmd_redo)right;
                        outFile = (cmd_atom)tmp->left;
                        File = outFile->node->data;
                    }else if(right->type == CMD_REDOR){
                        cmd_redo tmp = (cmd_redo)right;
                        outFile = (cmd_atom)tmp->left;
                        File = outFile->node->data;
                    }
                    fd = open(File, O_WRONLY|O_APPEND|O_CREAT|O_APPEND, 0777);
                    if(fd < 0)
                        exit(1);
                    dup2(fd, STDOUT_FILENO);
                    //printf("Redor running\n");
                    cmd_run(left);
                    if(fd != STDOUT_FILENO)
                        close(fd);
                    exit(1);
                }
                default:{
                    int status;
                    waitpid(pid, &status, 0);
                    if(right->type != CMD_ATOM)
                        cmd_run(right);
                    int err = WEXITSTATUS(status);
                    if(err)
                        printf("Error: %s\n.", strerror(err));
                }
            }
            break;
        }
        default:
            break;
    }
    exit(0);
}
