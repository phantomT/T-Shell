%{
//#define YYSTYPE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int yylex();
void yyerror(const char* msg);

cmd_t cmd;
extern cmd_t root;
%}

%union{
    int num;
    char *id;
    cmd_t cmd;
    node node;
}

%token<id> T_ARG

%type<cmd> command basic_command
%type<num> line
%type<node> args

%left '|' ';'
%left '&'
%right '>'
%right ">>"
%right '<'

%%

line            :  command '\n'	    		{ root = $1; $$ = 0; return 0; }
		        | '\n'       		        { root = 0; $$ = 0; return 0; }
                ;

command	        :  basic_command		    { $$ = $1;}
		        |  command ';' command  	{ $$ = cmd_list_new($1, $3);}
		        |  command '&'			    { $$ = cmd_back_new($1);}
		        |  command '|' command 		{ $$ = cmd_pipe_new($1, $3);}
		        |  command '<' command      { $$ = cmd_redi_new($1, $3, 0);}
		        |  command '>' command		{ $$ = cmd_redo_new($1, $3, 1);}
		        |  command ">>" command     { $$ = cmd_redor_new($1, $3, 1);}
		        ;

basic_command   :  T_ARG args			    { struct Node *t = make_node($1, $2); $$ = cmd_atom_new(t);}
		        ;

args		    : T_ARG args			    { $$ = make_node($1, $2);}
		        |                           { $$ = (struct Node *)0;}
		        ;

%%

void yyerror(const char* msg) {
    printf("%s\n",msg);
    exit(1);
}