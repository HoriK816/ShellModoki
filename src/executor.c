#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

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

        bool is_true = EvaluateConditionNode(if_node->condition);
        if(is_true)
        {
            ExecTree(if_node->process, symbol_table);
        }

        return;
    }
    else if(node->type == COMMAND)
    {
        command_node_t *command_node;
        command_node = (command_node_t*)node;
        ExecCommand(command_node);
        return;
    }
    else if(node->type == BINARY_OPERATION)
    {
        binary_operator_node_t *binary_node;
        binary_node = (binary_operator_node_t*)node;

        ExecBinaryOperator(binary_node);
        return;
    }
    else if(node->type ==  VARIABLE_DIFINITION)
    {
        variable_define_node_t *variable_node;
        variable_node = (variable_define_node_t*)node;

        StoreVaribale(variable_node, symbol_table);
        return;
    }

    
    for(int i=0;i<node->number_of_children;i++){
        ast_node_t *next_node;
        next_node  = (ast_node_t *)node->children[i];
        ExecTree(next_node, symbol_table);
    }
}

bool ExecBinaryOperator(binary_operator_node_t *binary_node)
{

    bool result;

    // left branch 
    pid_t status;
    int exit_code;
    if(binary_node->left->type == COMMAND){
        status = ExecCommand((command_node_t*)binary_node->left);

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
                        (binary_operator_node_t*)binary_node->left); 

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
        status = ExecCommand((command_node_t*)binary_node->right);
        exit_code = WEXITSTATUS(status);

        if(exit_code == 0 )
            result = true;
    }

    if(binary_node->right->type == BINARY_OPERATION){
        is_success = ExecBinaryOperator(
                        (binary_operator_node_t*)binary_node->right); 

        if(is_success)
            result = true;

    }

    return result;
}


bool EvaluateConditionNode(condition_node_t* condition)
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
    int operand1 = condition->operand1;
    int operand2 = condition->operand2;
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

pid_t ExecCommand(command_node_t *command_node)
{
    pid_t status;
    char *command_pointer[10];
    
    command_pointer[0] = command_node->command;

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
    return status;
}
