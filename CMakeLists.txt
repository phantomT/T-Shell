cmake_minimum_required(VERSION 3.21)
project(TShell C)

find_package(BISON)
find_package(FLEX)

set(CMAKE_C_STANDARD 11)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_LIBRARY(LEX_LIB l)

FLEX_TARGET(scanner scanner.l   ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
BISON_TARGET(parser parser.y    ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)

ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_executable(TShell main.c
        ${BISON_parser_OUTPUTS}
        ${FLEX_scanner_OUTPUTS}
        read_cmd.h main.h type_prompt.c read_cmd.c exec_cmd.c builtin_cmd.c sig_handle.c sig_handle.c)
