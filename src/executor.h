#pragma once

#include<sys/types.h>
#include<stdbool.h>
#include "parser.h"

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table);
bool ExecBinaryOperator(binary_operator_node_t *binary_node,
                        symbol_table_t* symbol_table);
pid_t ExecCommand(command_node_t *command_node,
                  symbol_table_t* symbol_table);
bool EvaluateConditionNode(condition_node_t* condition,
                           symbol_table_t *symbol_table);
void ExtractVariable(char **args, int number_of_args,
                     symbol_table_t *symbol_table);
