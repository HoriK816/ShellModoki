#pragma once

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
    char*                     operation;
    int                       value ;
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

int EvaluateArithmeticTree(arithmetic_node_t* node);
void DumpArithmeticTree(arithmetic_node_t* node, int level);
