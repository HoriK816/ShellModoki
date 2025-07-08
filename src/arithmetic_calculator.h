#pragma once
#include <stdbool.h>
#include "symbol_table.h"

typedef enum
{
    PLUS     = 1,
    MINUS    = 1,
    MULTIPLY = 2,
    DIVIDE   = 2,
    MOD      = 2,
    NUMBER   = 3,
}
priority_e;

typedef struct arithmetic_node_t
{
    char*                     operation; // i also use it as a valuable names
    int                       value ;
    bool                      is_variable;
    priority_e                priority;
    struct arithmetic_node_t* left_tree;
    struct arithmetic_node_t* right_tree;
}
arithmetic_node_t;


arithmetic_node_t* CreateArithmeticNode();

arithmetic_node_t* InsertOperand(char **tokens, int *current_cursor, 
                                 arithmetic_node_t* root, int number_of_tokens);

arithmetic_node_t* InsertOperator(char **tokens, int *current_cursor, 
                    arithmetic_node_t* root, int number_of_tokens);

arithmetic_node_t* BuildArithmeticTree(char **tokens, int arithmetic_cursor, 
                        arithmetic_node_t* root, int number_of_tokens);

int EvaluateArithmeticTree(arithmetic_node_t* node,
                           symbol_table_t *symbol_table);
void DumpArithmeticTree(arithmetic_node_t* node, int level);
