#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

#include "debug_functions.h"

// for debugging tokenizer 
void DumpTokenizeResult(char **tokens, int number_of_tokens)
{
    printf("there are %d tokens.\n", number_of_tokens);
    for(int i=0; i<number_of_tokens;i++){
        printf("%s\n", tokens[i]);
    }
}


// for debugging symbol tables
void DumpSymbolTable(symbol_table_t *symbol_table)
{
    printf("--------------symbol table--------------------\n");
    for(int i=0; i<symbol_table->number_of_records; i++)
    {
        printf("%s : %d\n", symbol_table->symbol_name[i],
                            *symbol_table->values[i]);
    }
   printf("-----------------------------------------------\n"); 
}

// for debugging
void DumpParseTree(ast_node_t *node, int level)
{
    // dump itself
    if(node->type == ROOT)
    {
        for(int i=0; i<level;i++)
            printf("\t");

        // root
        printf("root\n");
    }
    else if(node->type == COMMAND)
    {   
        // command
        command_node_t *command_node;
        command_node = (command_node_t*)node;
        for(int i=0; i<level;i++)
            printf("\t");
        printf("command name : %s\n", command_node->command);

        for(int j=0; j<command_node->number_of_args;j++)
        {
            for(int i=0; i<level;i++)
                printf("\t");
            printf("arg : %s\n", command_node->args[j]);
        }
        return;
    }
    else if(node->type == BINARY_OPERATION)
    {
        // && , || , ;
        binary_operator_node_t* binary_node;
        binary_node = (binary_operator_node_t*)node;
        
        for(int i=0; i<level;i++)
            printf("\t");
        printf("operator name : %s\n", binary_node->operation);
        level++; 

        // left
        DumpParseTree(binary_node->left, level);
        // right
        DumpParseTree(binary_node->right, level);
        return;
    }
    else if(node->type ==  VARIABLE_DIFINITION)
    {
        variable_define_node_t *variable_node;
        variable_node = (variable_define_node_t*)node;

        for(int i=0; i<level;i++)
            printf("\t");
        printf("variable name : %s\n", variable_node->variable_name);

        for(int i=0; i<level;i++)
            printf("\t");
        printf("value : %d\n", variable_node->value);
        return;
    }
    else if(node->type == IF)
    {
        if_node_t *if_node;
        if_node = (if_node_t *)node;
        
        for(int i=0; i<level;i++)
            printf("\t");
        printf("if\n");
        level++;

        DumpParseTree((ast_node_t*)if_node->condition, level);
        DumpParseTree(if_node->process, level);
        return;
    }
    else if(node->type == CONDITION)
    {
        condition_node_t *condition_node;
        condition_node = (condition_node_t *)node;

        for(int i=0; i<level;i++)
            printf("\t");
        printf("operator: %s\n", condition_node->operation);
        for(int i=0; i<level;i++)
            printf("\t");
        printf("operand1: %d\n", condition_node->operand1);
        for(int i=0; i<level;i++)
            printf("\t");
        printf("operand2: %d\n", condition_node->operand2);
        return;
    }

    else
    {
        // unknown
        printf("unknown type\n");
        return;
    }

    // dump children 
    for(int i=0; i<node->number_of_children; i++)
    {
        ast_node_t *next_node;
        next_node = (struct ast_node_t *)node->children[i];
        DumpParseTree(next_node, ++level);
    }
}
