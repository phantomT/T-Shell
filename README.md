# T-Shell project
This is a simplified version of shell on Linux.

这是一个**非常简易**的shell，是一个学习用的项目

其中使用`flex`和`bison`以及语法树构建的代码改自《工程化C》[课程源码模板](https://csslab-ustc.github.io/courses/sysprog/lab1/index.html)

## TODO
- [ ] `SIG`系统信号捕获与处理
- [ ] `>>`追加写入功能【现在还没搞懂怎么加入词法分析树】
## 已经实现功能
- 内建命令：`cd/about/history/exit/quit`等，`history`命令必须带参数执行
- Program指令：`ls/pwd/cat/grep`等在`PATH`中的程序
- 用`;`分段的多命令执行
- `< >`输入输出重定向
- `|`管道
- `&`后台运行
## 主要文件
- `main.c`主函数
- `main.h`主函数的头文件，包含大部分函数声明
- `scanner.l`使用`flex`编写的规则
- `parser.y`使用`bison`的分词器
- `read_cmd`包含命令树构建的代码
- `type_prompt.c`生成命令行开头的提示符
- `exec_cmd.c`包含主函数执行的执行命令函数和具体操作的执行命令函数
- `builtin_cmd.c`包含比较复杂的内建命令实现，如`cd`, `history`
## 最初始的设计框架
```
#define TRUE 1
while(TRUE){
    type_prompt();
    read_command(command, parameters);
    if(fork()!=0)
        waitpid(-1,&status,0);
    else
        execve(command,parameters,0);
}
```
> 即便把我关在果壳之中，仍然自以为无限空间之王
> 
> I could be bounded in a nutshell and count myself a king of infinite space.