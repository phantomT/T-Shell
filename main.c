#include "main.h"
#include "read_cmd.h"

struct passwd *pwd;
cmd_t root = 0;
char history_buff[50][128] = {};
int history_cnt = 0;
int history_queue_start = 0;
extern int yyparse();

int main() {
    while(1){
        type_prompt();
        yyparse();
        if(root == NULL) continue;
        //cmd_print(root);
        save_history(root);
        exec_cmd(root);
    }
    return 0;
}
