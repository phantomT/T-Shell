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

void history_cmd(node arg){
    int n = atoi(arg->data);
    if(arg->data == NULL) n = history_cnt;
    if(n >= history_cnt) n = history_cnt;
    //(start+n)%50-i
    for(int i = n-1; i >= 0; --i)
        printf("%d\t%s\n", n-i, history_buff[(history_queue_start+history_cnt-i-1)%50]);
}

void save_history(cmd_t _root){
    int cur_line;
    if(history_cnt == 50){
        cur_line = history_queue_start;
        ++history_queue_start;
        history_queue_start %= 50;
    }else{
        cur_line = history_cnt % 50;
        ++history_cnt;
    }

    memset(history_buff[cur_line], 0, sizeof(history_buff[cur_line]));
    write2history(_root, cur_line);
}

void write2history(cmd_t cmd, int cur_line){
    switch (cmd->type) {
        case CMD_ATOM:{
            cmd_atom t = (cmd_atom)cmd;
            node _node = t->node;
            while(_node){
                unsigned cur_len = strlen(history_buff[cur_line]);
                unsigned cmd_len = strlen(_node->data);
                snprintf(history_buff[cur_line]+cur_len, cmd_len+2, "%s ", _node->data);
                _node = _node->next;
            }
            break;
        }
        case CMD_LIST:{
            cmd_list t = (cmd_list)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;

            write2history(left, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 3, "; ");
            write2history(right, cur_line);
            break;
        }
        case CMD_BACK:{
            cmd_back t = (cmd_back)cmd;
            cmd_t back = t->back;

            write2history(back, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 3, " &");
            break;
        }
        case CMD_PIPE:{
            cmd_pipe t = (cmd_pipe)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;

            write2history(left, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 4, " | ");
            write2history(right, cur_line);
            break;
        }
        case CMD_REDI:{
            cmd_redi t = (cmd_redi)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;

            write2history(left, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 4, " < ");
            write2history(right, cur_line);
            break;
        }
        case CMD_REDO:{
            cmd_redo t = (cmd_redo)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;

            write2history(left, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 4, " > ");
            write2history(right, cur_line);
            break;
        }
        case CMD_REDOR:{
            cmd_redor t = (cmd_redor)cmd;
            cmd_t left = t->left;
            cmd_t right = t->right;

            write2history(left, cur_line);
            unsigned cur_len = strlen(history_buff[cur_line]);
            snprintf(history_buff[cur_line]+cur_len, 5, " >> ");
            write2history(right, cur_line);
            break;
        }
        default:
            break;
    }
}
