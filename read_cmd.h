//
// Created by tbb on 2021/12/5.
// ABSTRACT SYNTAX TREE
//

#ifndef TSHELL_READ_CMD_H
#define TSHELL_READ_CMD_H

typedef struct Node *node;
struct Node{
    char *data;
    struct Node *next;
};
struct Node *make_node(char *data, struct Node *next);

enum cmd_type_t{
    CMD_ATOM,
    CMD_BACK,
    CMD_LIST,
    CMD_PIPE,
    CMD_REDI,
    CMD_REDO,
    CMD_REDOR
};

typedef struct Cmd_t{
    enum cmd_type_t type;
}Cmd_t, *cmd_t;

//basic cmd
typedef struct Cmd_atom{
    enum cmd_type_t type;
    struct Node *node;
}Cmd_atom, *cmd_atom;
cmd_t cmd_atom_new(struct Node *);

//back cmd
typedef struct Cmd_back{
    enum cmd_type_t type;
    cmd_t back;
}Cmd_back, *cmd_back;
cmd_t cmd_back_new(cmd_t back);

//list cmd
typedef struct Cmd_list{
    enum cmd_type_t type;
    cmd_t left;
    cmd_t right;
}Cmd_list, *cmd_list;
cmd_t cmd_list_new(cmd_t left, cmd_t right);

//pipe cmd
typedef struct Cmd_pipe{
    enum cmd_type_t type;
    cmd_t left;
    cmd_t right;
}Cmd_pipe, *cmd_pipe;
cmd_t cmd_pipe_new(cmd_t left, cmd_t right);

//redi cmd
typedef struct Cmd_redi{
    enum cmd_type_t type;
    cmd_t left;
    cmd_t right;
    int fd;
}Cmd_redi, *cmd_redi;
cmd_t cmd_redi_new(cmd_t left, cmd_t right, int fd);

//redo cmd
typedef struct Cmd_redo{
    enum cmd_type_t type;
    cmd_t left;
    cmd_t right;
    int fd;
}Cmd_redo, *cmd_redo;
cmd_t cmd_redo_new(cmd_t left, cmd_t right, int fd);

//redo-re cmd
typedef struct Cmd_redor{
    enum cmd_type_t type;
    cmd_t left;
    cmd_t right;
    int fd;
}Cmd_redor, *cmd_redor;
cmd_t cmd_redor_new(cmd_t left, cmd_t right, int fd);

cmd_t cmd_redo_check(cmd_t left, cmd_t right, int fd, char* op);

//print AST
void cmd_print(cmd_t _cmd);

#endif //TSHELL_READ_CMD_H
