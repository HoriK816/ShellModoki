#pragma once

#define HOME_DIR "/home/horik/"
#define HOME_DIR_MAC "/Users/horik"
#define HOME_EXP_ENV "/2024/v24e3026"

enum node_type{
    ROOT,
    COMMAND, 
    BINARY_OPERATION,
    VARIABLE_DIFINITION,
};

enum read_mode{
    PARSE_COMMAND,
    PARSE_SEMICOLON,
    PARSE_AND,
    PARSE_OR,
    PARSE_VARIABLE_DIFINITION,
    PARSE_VARIABLE,
};

enum read_command_mode{
    READ_COMMAND_NAME,
    READ_COMMAND_ARGS,
};

typedef struct ast_node_t{ // base struct
    enum node_type type;
    int number_of_children;
    struct ast_node_t **children;
}ast_node_t;

typedef struct{
    ast_node_t node;
    ast_node_t *left;
    ast_node_t *right;
    char *operation;
    bool is_true;
}binary_operator_node_t;

typedef struct{ // terminal node
    ast_node_t node;
    char *command;
    char **args;
    int number_of_args;
}command_node_t;

typedef struct{
    ast_node_t node;
    char *variable_name;
    int value; // myshell supports only integer as a value of variables.
}variable_define_node_t;


