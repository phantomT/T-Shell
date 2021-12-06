#include "main.h"
#include "read_cmd.h"

struct passwd *pwd;
cmd_t root = 0;
extern int yyparse();

int main() {
    while(1){
        type_prompt();
        yyparse();
        if(root == NULL) continue;
        //cmd_print(root);
        exec_cmd(root);
    }
    return 0;
}
