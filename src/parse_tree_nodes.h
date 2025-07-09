#pragma once

#include "parser.h"
#include "shell_modoki.h"

/* utility */
ast_node_t* CreateSubTreeNode();
condition_node_t* CreateConditionNode();
if_node_t* CreateIFNode();
command_node_t* CreateCommandNode(char *token);
variable_define_node_t* CreateVariableDefinitionNode();
binary_operator_node_t *CreateBinaryOperatorNode();
while_node_t* CreateWHILENode();
if_node_t* CreateELSENode();
