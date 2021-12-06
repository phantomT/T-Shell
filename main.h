//
// Created by tbb on 2021/12/5.
//

#ifndef TSHELL_MAIN_H
#define TSHELL_MAIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include "read_cmd.h"

#define MAX_PROMPT 1024
#define NEW(p) \
do{            \
    p = malloc(sizeof(*p)); \
}while(0)

extern struct passwd *pwd;
extern cmd_t root;

void type_prompt(void);
int cd_cmd(node _argc);
int history_cmd(node _argc);
void exec_cmd(cmd_t _cmd);
void cmd_run(cmd_t _cmd);

#endif //TSHELL_MAIN_H
