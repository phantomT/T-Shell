//
// Created by tbb on 2021/12/5.
//
#include "main.h"

char hostName[max_name_len];
char pathName[max_path_len];

void type_prompt(void){
    char prompt[MAX_PROMPT];

    pwd = getpwuid(getuid());
    getcwd(pathName, max_path_len);
    if(gethostname(hostName, max_name_len) == 0)
        sprintf(prompt,"\033[1;33mT_bb_%s@%s:\033[1;33m",pwd->pw_name,hostName);
    else
        sprintf(prompt,"\033[1;33mT_bb_%s@unknown:\033[1;33m",pwd->pw_name);

    unsigned prompt_len = strlen(prompt);
    unsigned pw_dir_len = strlen(pwd->pw_dir);
    if(strlen(pathName) < pw_dir_len ||
       strncmp(pathName, pwd->pw_dir, pw_dir_len) != 0)
        sprintf(prompt + prompt_len, "\033[0;35m%s\033[0m", pathName);
    else
        sprintf(prompt + prompt_len, "\033[0;35m~%s\033[0m", pathName + pw_dir_len);

    prompt_len = strlen(prompt);
    if(geteuid() == 0)
        snprintf(prompt+prompt_len, 2, "#");
    else
        snprintf(prompt+prompt_len, 2, "$");

    printf("%s", prompt);
}
