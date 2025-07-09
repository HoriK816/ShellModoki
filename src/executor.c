#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#include "arithmetic_calculator.h"
#include "executor.h"
#include "history.h"

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table)
{

    if(node->type == ROOT)
    {

    }
    else if(node->type == IF)
    {
        if_node_t* if_node;
        if_node = (if_node_t*)node;

        bool is_true = EvaluateConditionNode(if_node->condition, 
                                             symbol_table);
        if(is_true)
        {
            ExecTree(if_node->process, symbol_table);
            if_node->is_executed = true;
        }

        return;
    }
    else if(node->type == ELSE)
    {
        if_node_t* else_node;
        else_node = (if_node_t*)node;

        ExecTree(else_node->process, symbol_table);
        else_node->is_executed = true;

    }
    else if(node->type == WHILE)
    {
        while_node_t* while_node;
        while_node = (while_node_t*)node;

        bool is_true = EvaluateConditionNode(while_node->condition,
                                             symbol_table);
        while(is_true)
        {
            ExecTree(while_node->process, symbol_table);

            is_true = EvaluateConditionNode(while_node->condition,
                                            symbol_table);
        }
        return;
    }
    else if(node->type == COMMAND)
    {
        command_node_t *command_node;
        command_node = (command_node_t*)node;
        ExecCommand(command_node, symbol_table);
        return;
    }
    else if(node->type == BINARY_OPERATION)
    {
        binary_operator_node_t *binary_node;
        binary_node = (binary_operator_node_t*)node;

        ExecBinaryOperator(binary_node, symbol_table);
        return;
    }
    else if(node->type ==  VARIABLE_DIFINITION)
    {
        variable_define_node_t *variable_node;
        variable_node = (variable_define_node_t*)node;

        StoreVariable(variable_node, symbol_table);
        return;
    }

    int cursor = 0;
    while(cursor < node->number_of_children)
    {
        ast_node_t *next_node;
        next_node  = (ast_node_t *)node->children[cursor];
        if(next_node->type == IF)
        {
            cursor = ProcessIFNode(node, cursor, 
                              node->number_of_children, symbol_table);
        }
        else
        {
            ExecTree(next_node, symbol_table);
            cursor++;
        }
    }
}

int ProcessIFNode(ast_node_t* node, int base_index, int number_of_children,
                  symbol_table_t* symbol_table)
{
    /* check if coverage */
    int number_of_if_nodes = 0; 
    for(int i=base_index; i<number_of_children; i++)
    {
        ast_node_t* temp_node = node->children[i];
        if(! ((temp_node->type == IF) || (temp_node->type == ELSE)))
            break;

        number_of_if_nodes++;
    }

    /* exec if nodes */
    for(int i=0; i<number_of_if_nodes; i++)
    {
        ExecTree((ast_node_t*)node->children[base_index+i], symbol_table);
        ast_node_t* temp_node   = node->children[base_index + i];
        if_node_t* temp_if_node = (if_node_t*)temp_node;
        if(temp_if_node->is_executed)
            break;
    }

    /* reset is_executed */
    for(int i=0; i<number_of_if_nodes; i++)
    {
        ast_node_t* temp_node   = node->children[base_index + i];
        if_node_t* temp_if_node = (if_node_t*)temp_node;
        temp_if_node->is_executed = false;

    }

    return base_index + number_of_if_nodes;
}


bool ExecBinaryOperator(binary_operator_node_t *binary_node,
                        symbol_table_t* symbol_table)
{
    bool result;

    // left branch 
    pid_t status;
    int exit_code;
    if(binary_node->left->type == COMMAND){
        status = ExecCommand((command_node_t*)binary_node->left, symbol_table);

        exit_code = WEXITSTATUS(status);
        if(exit_code == 0){
            if(strcmp(binary_node->operation, "||") == 0)
                return true;
        }else{
           if(strcmp(binary_node->operation, "&&") == 0)
                return false;
        }
    }

    bool is_success;
    if(binary_node->left->type == BINARY_OPERATION){
        is_success = ExecBinaryOperator(
                        (binary_operator_node_t*)binary_node->left,
                        symbol_table); 

        if(is_success){
            if(strcmp(binary_node->operation, "||") == 0)
                return true;
        }else{
            if(strcmp(binary_node->operation, "&&") == 0)
                return false;
        }

    }


    // right branch
    if(binary_node->right->type == COMMAND){
        status = ExecCommand((command_node_t*)binary_node->right,
                             symbol_table);
        exit_code = WEXITSTATUS(status);

        if(exit_code == 0 )
            result = true;
    }

    if(binary_node->right->type == BINARY_OPERATION){
        is_success = ExecBinaryOperator(
                        (binary_operator_node_t*)binary_node->right,
                        symbol_table); 

        if(is_success)
            result = true;

    }

    return result;
}


bool EvaluateConditionNode(condition_node_t* condition,
                           symbol_table_t *symbol_table)
{
    /* set the corresponded operator */
    enum comparison_operator operator;
    
    if(strcmp(condition->operation, "-eq") == 0)
        operator = EQUAL;

    if(strcmp(condition->operation, "-ne") == 0)
        operator = NOT_EQUAL;

    if(strcmp(condition->operation, "-gt") == 0)
        operator = GREATER_THAN;

    if(strcmp(condition->operation, "-lt") == 0)
        operator = LESSER_THAN;

    if(strcmp(condition->operation, "-ge") == 0)
        operator = GREATER_EQUAL;

    if(strcmp(condition->operation, "-le") == 0)
        operator = LESSER_EQUAL;


    /* evaluate given operands */
    arithmetic_node_t *operand1_root = NULL;
    arithmetic_node_t *operand2_root = NULL;
    
    int cursor = 0;
    operand1_root = BuildArithmeticTree(condition->operand1, cursor, 
                                        operand1_root,
                                        condition->number_of_operand1_tokens); 
    /*DumpArithmeticTree(operand1_root, 0, symbol_table);*/
    cursor = 0;
    operand2_root = BuildArithmeticTree(condition->operand2, cursor,
                                        operand2_root,
                                        condition->number_of_operand2_tokens);
    /*DumpArithmeticTree(operand2_root, 0, symbol_table);*/

    int operand1 = EvaluateArithmeticTree(operand1_root, symbol_table);
    int operand2 = EvaluateArithmeticTree(operand2_root, symbol_table);
    bool is_true = false;
    switch(operator)
    {
        case EQUAL:
            if(operand1 == operand2)
                is_true = true;
            break;

        case NOT_EQUAL:
            if(operand1 != operand2)
                is_true = true;
            break;

        case GREATER_THAN:
            if(operand2 < operand1)
                is_true = true;
            break;

        case LESSER_THAN:
            if(operand1 <= operand2)
                is_true = true;
            break;

        case GREATER_EQUAL:
            if(operand2 <= operand1)
                is_true = true;
            break;

        case LESSER_EQUAL:
            if(operand1 <= operand2)
                is_true = true;
            break;
    }

    return is_true;
}

pid_t ExecCommand(command_node_t *command_node, symbol_table_t* symbol_table)
{
    pid_t status;
    char *command_pointer[10];
    
    command_pointer[0] = command_node->command;

    /* prepare backups */
    char ** backup_args = (char**)malloc(sizeof(char*)*100);
    for(int i=0; i<100; i++)
        backup_args[i] = (char*)malloc(sizeof(char) * 100);
    for(int i=0; i<command_node->number_of_args; i++)
        strcpy(backup_args[i], command_node->args[i]);

    ExtractVariable(command_node->args,
                    command_node->number_of_args,
                    symbol_table);

    for(int i=0;i<(command_node->number_of_args);i++)
        command_pointer[i+1] = (command_node->args)[i];

    command_pointer[command_node->number_of_args+1] = NULL;

    // exit
    if(strcmp(command_pointer[0], "exit") == 0)
        exit(0);

    // history
    if(strcmp(command_pointer[0], "history") == 0)
        PrintHistory();

    // change directory
    if(strcmp(command_pointer[0], "cd") == 0)
    {
        if(command_pointer[1] == NULL)
        {
            chdir(HOME_EXP_ENV);
        }else{
            chdir(command_pointer[1]);
        }
    }

    if(fork() == 0){
        execvp(command_pointer[0], command_pointer);
        fprintf(stderr, "%s is not found \n", command_pointer[0]);
        exit(1);
    }else{
        wait(&status);

    }
    
    /* recover args */
    for(int i=0; i<command_node->number_of_args; i++)
        strcpy(command_node->args[i], backup_args[i]);

    return status;
}

void ExtractVariable(char **args, int number_of_args,
                     symbol_table_t *symbol_table)
{
    for(int i=0; i<number_of_args; i++)
    {
        // discard '$'
        char *name = strchr(args[i], '$');
        if(name == NULL)
            continue;

        /* truncate the first '$' */
        name++;

        for(int j=0; j<symbol_table->number_of_records;j++)
        {
            if(strcmp(name, symbol_table->symbol_name[j]) == 0)
            {
                sprintf(args[i], "%d", *symbol_table->values[j]);
                break;
            }
        }
    }
}
