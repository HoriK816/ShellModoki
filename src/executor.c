#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#include "executor.h"
#include "history.h"

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table)
{

    if(node->type == ROOT){

    }else if(node->type == COMMAND){
        command_node_t *command_node;
        command_node = (command_node_t*)node;
        ExecCommand(command_node);
        return;

    }else if(node->type == BINARY_OPERATION){
        binary_operator_node_t *binary_node;
        binary_node = (binary_operator_node_t*)node;

        ExecBinaryOperator(binary_node);
        return;

    }else if(node->type ==  VARIABLE_DIFINITION){
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
