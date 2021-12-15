//
// Created by tbb on 2021/12/6.
//
#include "main.h"

node make_node(char *data, node next){
    struct Node *t = malloc(sizeof(*t));
    t->data = data;
    t->next = next;
    return t;
}

// constructor of each AST node
cmd_t cmd_atom_new(struct Node *node){
    cmd_atom cmd;
    NEW(cmd);
    cmd->type = CMD_ATOM;
    cmd->node = node;
    return (cmd_t)cmd;
}

cmd_t cmd_back_new(cmd_t back){
    cmd_back cmd;
    NEW(cmd);
    cmd->type = CMD_BACK;
    cmd->back = back;
    return (cmd_t)cmd;
}

cmd_t cmd_list_new(cmd_t left, cmd_t right){
    cmd_list cmd;
    NEW(cmd);
    cmd->type = CMD_LIST;
    cmd->left = left;
    cmd->right = right;
    return (cmd_t)cmd;
}

cmd_t cmd_pipe_new(cmd_t left, cmd_t right){
    cmd_pipe cmd;
    NEW(cmd);
    cmd->type = CMD_PIPE;
    cmd->left = left;
    cmd->right = right;
    return (cmd_t)cmd;
}

cmd_t cmd_redi_new(cmd_t left, cmd_t right, int fd){
    cmd_redi cmd;
    NEW(cmd);
    cmd->type = CMD_REDI;
    cmd->left = left;
    cmd->right = right;
    cmd->fd = fd;
    return (cmd_t)cmd;
}

cmd_t cmd_redo_new(cmd_t left, cmd_t right, int fd){
    cmd_redi cmd;
    NEW(cmd);
    cmd->type = CMD_REDO;
    cmd->left = left;
    cmd->right = right;
    cmd->fd = fd;
    return (cmd_t)cmd;
}

cmd_t cmd_redor_new(cmd_t left, cmd_t right, int fd){
    cmd_redi cmd;
    NEW(cmd);
    cmd->type = CMD_REDOR;
    cmd->left = left;
    cmd->right = right;
    cmd->fd = fd;
    return (cmd_t)cmd;
}

cmd_t cmd_redo_check(cmd_t left, cmd_t right, int fd, char* op){
    if(strcmp(op, ">>") == 0)
        return cmd_redor_new(left, right, fd);
    else
        return cmd_redo_new(left, right, fd);
}

void cmd_print(cmd_t _cmd){
    switch (_cmd->type) {
        case CMD_ATOM:{
            cmd_atom t = (cmd_atom)_cmd;
            node _node = t->node;
            printf("ATOM ");
            while(_node){
                printf("%s ", _node->data);
                _node = _node->next;
            }
            break;
        }
        case CMD_LIST:{
            printf("enter\n");
            cmd_list t = (cmd_list)_cmd;
            printf("list\n");
            cmd_t left = t->left;
            cmd_t right = t->right;

            cmd_print(left);
            printf("; ");
            cmd_print(right);
            break;
        }
        case CMD_BACK:{
            printf("enter\n");
            cmd_back t = (cmd_back)_cmd;
            printf("go back\n");
            cmd_t back = t->back;

            cmd_print(back);
            printf("; ");
            break;
        }
        case CMD_PIPE:{
            printf("enter\n");
            cmd_pipe t = (cmd_pipe)_cmd;
            printf("pipe\n");
            cmd_t left = t->left;
            cmd_t right = t->right;

            cmd_print(left);
            printf("; ");
            cmd_print(right);
            break;
        }
        case CMD_REDI:{
            printf("redi\n");
            cmd_redi t = (cmd_redi)_cmd;
            printf("redi\n");
            cmd_t left = t->left;
            cmd_t right = t->right;
            int _fd = t->fd;
            printf("%d ", _fd);

            cmd_print(left);
            printf("; ");
            cmd_print(right);
            break;
        }
        case CMD_REDO:{
            printf("redo\n");
            cmd_redo t = (cmd_redo)_cmd;
            printf("redo\n");
            cmd_t left = t->left;
            cmd_t right = t->right;
            int _fd = t->fd;
            printf("%d ", _fd);

            cmd_print(left);
            printf("; ");
            cmd_print(right);
            break;
        }
        case CMD_REDOR:{
            printf("redor\n");
            cmd_redor t = (cmd_redor)_cmd;
            printf("redor\n");
            cmd_t left = t->left;
            cmd_t right = t->right;
            int _fd = t->fd;
            printf("%d ", _fd);

            cmd_print(left);
            printf("; ");
            cmd_print(right);
            break;
        }
        default:
            break;
    }
}