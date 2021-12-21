#include "main.h"
#include "read_cmd.h"

struct passwd *pwd;
cmd_t root = 0;
char history_buff[50][128] = {};
int history_cnt = 0;
int history_queue_start = 0;
sigset_t sigset;
extern int yyparse();

int main() {
    printf("Hi, Welcome to the Machine!\n");
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    while(1){
        type_prompt();
        yyparse();
        if(root == NULL) continue;
        //cmd_print(root);
        //save_history(root);
        exec_cmd(root);
    }
    return 0;
}
