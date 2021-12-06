//
// Created by tbb on 2021/12/6.
//
#include "main.h"

int cd_cmd(node arg){
    if(arg->next) {
        printf("Command numbers Error\n");
        return 0;
    }
    char* cd_path = NULL;
    unsigned path_len = strlen(arg->data) + 1;
    if(arg->data[0] == '~'){
        path_len += strlen(pwd->pw_dir);
        cd_path = malloc(path_len);
        if(cd_path == NULL){
            printf("cd: malloc failed.\n");
            exit(1);
        }
        strncpy(cd_path, pwd->pw_dir, strlen(pwd->pw_dir)+1);
        strncpy(cd_path+strlen(pwd->pw_dir), arg->data, strlen(arg->data)+1);
    }else{
        cd_path = malloc(path_len);
        if(cd_path == NULL){
            printf("cd: malloc failed.\n");
            exit(1);
        }
        strncpy(cd_path, arg->data, strlen(arg->data)+1);
    }
    if(chdir(cd_path) != 0)
        fprintf(stderr, "T-bb: can't cd %s.\n", cd_path);
    free(cd_path);
    return 0;
}

int history_cmd(node argc){

}
