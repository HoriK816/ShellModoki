#pragma once
#include <stdio.h>
#include <stdbool.h>

#define HOME_DIR "/home/horik/"
#define HOME_DIR_MAC "/Users/horik"
#define HOME_EXP_ENV "/2024/v24e3026"

/* AST nodes */
enum node_type
{
    ROOT,
    COMMAND, 
    BINARY_OPERATION,
    VARIABLE_DIFINITION,
    IF,
    WHILE,
    CONDITION,
};

/* base struct */
typedef struct ast_node_t
{
    enum node_type      type; 
    struct ast_node_t** children;
    int                 number_of_children;
}
ast_node_t;

typedef struct
{
    ast_node_t          node; // inherit base struct 
    ast_node_t*         left;
    ast_node_t*         right;
    char*               operation;
}
binary_operator_node_t;

typedef struct
{
    ast_node_t          node;              // base struct
    char**              operand1;
    char*               operation;
    char**              operand2;
    int                 number_of_operand1_tokens;
    int                 number_of_operand2_tokens;
    bool                is_true;
}
condition_node_t;

typedef struct
{
    ast_node_t          node;              // base struct
    condition_node_t*   condition;
    ast_node_t*         process;
}
if_node_t;

typedef struct
{
    ast_node_t          node;
    condition_node_t*   condition;
    ast_node_t*         process;
}
while_node_t;

typedef struct
{
    ast_node_t          node;
    char*               command;
    char**              args;
    int                 number_of_args;
}
command_node_t;

typedef struct
{
    ast_node_t          node;
    char*               variable_name;

    /* ShellModoki supports only integer as variables */
    int                 value;
}
variable_define_node_t;

/* parse mode */
enum read_mode
{
    PARSE_COMMAND,
    PARSE_SEMICOLON,
    PARSE_AND,
    PARSE_OR,
    PARSE_VARIABLE_DIFINITION,
    PARSE_VARIABLE,
    PARSE_IF,
    PARSE_WHILE,
};

enum read_command_mode
{
    READ_COMMAND_NAME,
    READ_COMMAND_ARGS,
};

enum comparison_operator
{
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    LESSER_THAN,
    GREATER_EQUAL,
    LESSER_EQUAL
};

char* GetUserInputLine();
size_t CountCommand(char **commands);
bool IsPromptEnded(char **tokens, int number_of_tokens);
