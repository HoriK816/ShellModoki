#include <stdlib.h>
#include "parse_tree_nodes.h"

ast_node_t* CreateSubTreeNode()
{
    /* prepare the subtree root node */
    ast_node_t *subtree_node;
    subtree_node = (ast_node_t*)malloc(sizeof(ast_node_t));
    if(subtree_node == NULL)
    {
        fprintf(stderr, "could not sufficient memory for ast_node");
        exit(EXIT_FAILURE);
    }

    subtree_node->type = ROOT;
    subtree_node->number_of_children = 0;

    // 100 is a tentative value
    subtree_node->children = (ast_node_t**)malloc(sizeof(ast_node_t*) * 100);
    if(subtree_node->children == NULL)
    {
        fprintf(stderr, "could not sufficient memory for ast_node");
        exit(EXIT_FAILURE);
    }

    return subtree_node;
}

condition_node_t* CreateConditionNode()
{

    condition_node_t *condition_node 
        = (condition_node_t*)malloc(sizeof(condition_node_t));
    if(condition_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for condition node");
        exit(EXIT_FAILURE);
    }

    condition_node->node.type = CONDITION;

    condition_node->operation = (char*)malloc(sizeof(char) * 10); 
    if(condition_node->operation == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for condition node");
        exit(EXIT_FAILURE);
    }

    return condition_node;
}

if_node_t* CreateIFNode()
{
    if_node_t *if_node = (if_node_t*)malloc(sizeof(if_node_t));
    if(if_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for if node");
        exit(EXIT_FAILURE);
    }
    
    (if_node->node).type = IF;

    if_node->process = (ast_node_t*)malloc(sizeof(ast_node_t));
    if_node->process->type = ROOT;
    if_node->process->number_of_children = 0;
    if_node->process->children = (ast_node_t**)malloc(sizeof(ast_node_t*)*10);
    
    return if_node;
}

while_node_t* CreateWHILENode()
{
    while_node_t *while_node = (while_node_t*)malloc(sizeof(while_node_t));
    if(while_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for while node");
        exit(EXIT_FAILURE);
    }

    (while_node->node).type = WHILE;

    while_node->process = (ast_node_t*)malloc(sizeof(ast_node_t));
    while_node->process->type = ROOT;
    while_node->process->number_of_children = 0;
    while_node->process->children = (ast_node_t**)malloc(sizeof(ast_node_t*)*10);

    return while_node;
}

command_node_t* CreateCommandNode(char *token)
{
    command_node_t *command_node;

    command_node = (command_node_t*)malloc(sizeof(command_node_t));
    if(command_node == NULL)
    {
        fprintf(stderr,
                "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }

    command_node->command = token;
    command_node->number_of_args = 0;

    // 50 is a tentetive value
    command_node->args = (char **)malloc(sizeof(char*)*50);
    if(command_node->args == NULL)
    {
        fprintf(stderr,
                "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }
    (command_node->node).type = COMMAND;

    return command_node;
}

variable_define_node_t* CreateVariableDefinitionNode()
{
    variable_define_node_t *variable_node;
    variable_node = (variable_define_node_t*)malloc(
                                    sizeof(variable_define_node_t));
    if(variable_node == NULL)
    {
        fprintf(stderr, "could not allcate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }
    (variable_node->node).type = VARIABLE_DIFINITION;

    return variable_node;
}

binary_operator_node_t *CreateBinaryOperatorNode()
{
    binary_operator_node_t *binary_node;

    binary_node = (binary_operator_node_t*)malloc(sizeof(binary_operator_node_t));
    if(binary_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }

    (binary_node->node).type = BINARY_OPERATION;

    binary_node->operation = (char*)malloc(sizeof(char) * 5);
    if(binary_node->operation == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }
    
    binary_node->right = (ast_node_t*)malloc(sizeof(ast_node_t));
    if(binary_node->right == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for ast node");
        exit(EXIT_FAILURE);
    }

    return binary_node;
}
