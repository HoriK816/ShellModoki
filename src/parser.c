#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

#include "parser.h"
#include "parse_tree_nodes.h"


int BuildParseTree(char **tokens, ast_node_t *node,
                   int current_cursor, int number_of_tokens,
                   symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    /* prepare the subtree root node */
    ast_node_t *subtree_node = CreateSubTreeNode();

    enum read_mode mode;
    while(cursor < number_of_tokens)
    {
        // TODO: This judgement should be separated int a function.
        if((strcmp(tokens[cursor], "fi") == 0)
                || (strcmp(tokens[cursor], "done") == 0))
        {
            // dont't consume "fi" or "done", because the parent function consumes it.
            // printf(" detect the end " );
            break;
        }

        mode = DecideNextMode(tokens, cursor, number_of_tokens);
        switch(mode)
        {
            /* leaf nodes */
            case PARSE_COMMAND:
                cursor = ParseCommand(tokens, subtree_node, cursor,
                                      number_of_tokens, symbol_table);
                break;

            case PARSE_VARIABLE_DIFINITION:
                cursor = ParseVaribleDifinition(tokens, subtree_node, cursor,
                                                number_of_tokens);
                break;

            /* binary operators */
            case PARSE_VARIABLE:
                ExtractVariable(tokens, cursor, symbol_table);
                break;

            case PARSE_SEMICOLON:
                break;

            case PARSE_AND:
                cursor = ParseAND(tokens, subtree_node, cursor,
                                  number_of_tokens, symbol_table); 
                break;

            case PARSE_OR:
                cursor = ParseOR(tokens, subtree_node, cursor,
                                 number_of_tokens, symbol_table);
                break;

            /* if statement */
            case PARSE_IF:
                cursor = ParseIF(tokens, subtree_node, cursor,
                                 number_of_tokens, symbol_table);
                break;

            /* while statement */
            case PARSE_WHILE:
                cursor = ParseWHILE(tokens, subtree_node, cursor, 
                                    number_of_tokens,symbol_table);
                break;
        }
    }

    /* concat this subtree to the parent node*/
    node->children[node->number_of_children] = subtree_node;
    node->number_of_children++;

    return cursor;
}

enum read_mode DecideNextMode(char **tokens, int cursor, int number_of_tokens)
{
    enum read_mode next_mode;
    char *token = tokens[cursor];

    if(strcmp(token, ";") == 0)
        next_mode = PARSE_SEMICOLON;

    else if(strcmp(token, "&&") == 0)
        next_mode = PARSE_AND;

    else if(strcmp(token, "||") == 0)
        next_mode = PARSE_OR;

    else if(strcmp(token, "if") == 0)
        next_mode = PARSE_IF;
    
    else if(strcmp(token, "while") == 0)
        next_mode = PARSE_WHILE;

    else if(cursor < number_of_tokens-1)
    {
        if(strcmp(tokens[cursor+1], "=") == 0)
            next_mode = PARSE_VARIABLE_DIFINITION;
        else if(token[0] == '$')
            next_mode = PARSE_VARIABLE;
        else
            next_mode = PARSE_COMMAND;
    }
    else
    {
        if(token[0] == '$')
            next_mode = PARSE_VARIABLE;
        else
            next_mode = PARSE_COMMAND;
    }

    return next_mode;

}

void ExtractVariable(char **tokens, int current_cursor,
                     symbol_table_t *symbol_table)
{
    // discard '$'
    char *name = strchr(tokens[current_cursor], '$');
    
    if(name == NULL)
        return;
    name++;

    for(int i=0; i<symbol_table->number_of_records;i++)
    {
        if(strcmp(name, symbol_table->symbol_name[i]) == 0)
            sprintf(tokens[current_cursor], "%d", *symbol_table->values[i]);
    }
}

int ParseCommand(char **tokens, ast_node_t *node,
                 int current_cursor, int number_of_tokens,
                 symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    enum read_command_mode mode = READ_COMMAND_NAME;
    command_node_t *command_node = NULL;
    for(int i=cursor; i<number_of_tokens; i++)
    {
        char* token = tokens[cursor];
        if(token == NULL)
            break;
        if(strcmp(token, ";")==0)
            break;
        if(strcmp(token, "||")==0)
            break;
        if(strcmp(token, "&&")==0)
            break;
        if(strcmp(token, "fi")==0)
            break;
        if(strcmp(token, "done")==0)
            break;


        if(mode == READ_COMMAND_NAME)
        {
            command_node = CreateCommandNode(token);

            // add this node to AST tree
            if(node->type  == BINARY_OPERATION)
            {
                binary_operator_node_t* binary_node 
                                        = (binary_operator_node_t*)node;
                binary_node->right = (struct ast_node_t*)command_node;
                
                cursor++;
            }
            else
            {
                int add_position = node->number_of_children;
                node->children[add_position] = (struct ast_node_t*)command_node;
                node->number_of_children++;

                cursor++;
            }
        }
        else if(mode == READ_COMMAND_ARGS)
        {
            ExtractVariable(tokens, cursor, symbol_table);

            // record args 
            int add_position = command_node->number_of_args;
            command_node->args[add_position] = token; 
            command_node->number_of_args++;

            cursor++;
        }

        if(mode == READ_COMMAND_NAME)
            mode = READ_COMMAND_ARGS;
    }
    return cursor;
}

int ParseAND(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens,
             symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    /* prepare an AND node */
    binary_operator_node_t *and_node = CreateBinaryOperatorNode();
    sprintf(and_node->operation, "&&");
    
    // left branch
    int last = node->number_of_children-1;
    and_node->left = node->children[last]; 
    node->children[last] = (ast_node_t *)and_node;

    /* consume an "&&" token */
    cursor += 1;  

    // right branch
    enum read_mode mode 
        = DecideNextMode(tokens, cursor, number_of_tokens);

    switch(mode)
    {
        case PARSE_COMMAND:
            cursor = ParseCommand(tokens, (ast_node_t*)and_node, cursor, 
                                           number_of_tokens, symbol_table);
            break;
        case PARSE_VARIABLE_DIFINITION:
            cursor = ParseVaribleDifinition(tokens, (ast_node_t*)and_node,
                                            cursor, number_of_tokens);
            break;
        default:
            break;
    }
    return cursor;
}

int ParseOR(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens,
             symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    /* prepare a OR node */
    binary_operator_node_t *or_node = CreateBinaryOperatorNode();
    (or_node->node).type = BINARY_OPERATION;

    // left branch
    int last = node->number_of_children-1;
    or_node->left = node->children[last]; 
    node->children[last] = (ast_node_t *)or_node;

    // consume an "||" token 
    cursor += 1;  

    // right branch
    enum read_mode mode 
        = DecideNextMode(tokens, cursor, number_of_tokens);

    switch(mode)
    {
        case PARSE_COMMAND:
            cursor = ParseCommand(tokens, (ast_node_t*)or_node, cursor, 
                                           number_of_tokens, symbol_table);
            break;
        case PARSE_VARIABLE_DIFINITION:
            cursor = ParseVaribleDifinition(tokens, (ast_node_t*)or_node,
                                            cursor, number_of_tokens);
            break;
        default:
            break;
    }
    return cursor;
}

int ParseVaribleDifinition(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens)
{
    int cursor = current_cursor;
    variable_define_node_t *variable_node = CreateVariableDefinitionNode();
    
    variable_node->variable_name = tokens[cursor];
    variable_node->value = atoi(tokens[cursor+2]);

    // add this node to AST tree
    if(node->type  == BINARY_OPERATION)
    {
        binary_operator_node_t* binary_node 
                                = (binary_operator_node_t*)node;
        binary_node->right = (struct ast_node_t*)variable_node;
    }
    else
    {
        int add_position = node->number_of_children;
        node->children[add_position] = (struct ast_node_t*)variable_node;
        node->number_of_children++;
    }
    // there are "variable_name", "=" and "value" , so add 3
    cursor += 3;

    return cursor;
}

int ParseIF(char **tokens, ast_node_t *node, 
            int current_cursor, int number_of_tokens,
            symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    if_node_t *if_node = CreateIFNode();

    // consume an "if" token
    cursor += 1;

    cursor = ParseCondition(tokens, (ast_node_t*)if_node, cursor,
                            number_of_tokens);
    
    // consume ';' "then" tokens
    cursor += 2;

    cursor = BuildParseTree(tokens, if_node->process, cursor,
                            number_of_tokens, symbol_table);

    // consume an "fi" token
    cursor += 1;
    
    node->children[node->number_of_children] = (ast_node_t*)if_node;
    node->number_of_children++;

    return cursor;
}

int ParseWHILE(char **tokens, ast_node_t *node, 
            int current_cursor, int number_of_tokens,
            symbol_table_t *symbol_table)
{
    int cursor = current_cursor;

    while_node_t *while_node = CreateWHILENode();

    // consume an "while" token
    cursor += 1;

    cursor = ParseCondition(tokens, (ast_node_t*)while_node, cursor,
                            number_of_tokens);

    // consume an "do" token
    cursor += 1;

    cursor = BuildParseTree(tokens, while_node->process, cursor,
                            number_of_tokens, symbol_table);

    // consume an "done" token
    cursor += 1;

    node->children[node->number_of_children] = (ast_node_t*)while_node;
    node->number_of_children++;

    return cursor;
}

int ParseCondition(char **tokens, ast_node_t *node, 
                   int current_cursor, int number_of_tokens)
{
    int cursor = current_cursor;

    /* parse an condition */

    condition_node_t *condition_node = CreateConditionNode();
    cursor += 1;                 // consume '['  

    /* operand 1 */
    cursor = ParseConditionOperand(tokens, condition_node->operand1, cursor,
                                   &condition_node->number_of_operand1_tokens);

    /* comparison operation */
    condition_node->operation = tokens[cursor];
    int len = strlen(tokens[cursor+1]);
    strncat(condition_node->operation, tokens[cursor+1], len);
    cursor += 2;                 // consume comparison operation

    /* operand 2 */
    cursor = ParseConditionOperand(tokens, condition_node->operand2, cursor,
                                   &condition_node->number_of_operand2_tokens);

    condition_node->is_true  = false;  
    cursor += 1;                 // consume ']'

    /* concatenate it to the parent node */
    if(node->type == IF)
    {
        if_node_t *if_node = (if_node_t*)node;
        if_node->condition = condition_node;
    }
    else if(node->type == WHILE)
    {
        while_node_t* while_node = (while_node_t*)node;
        while_node->condition = condition_node;
    }

    return cursor;
}

int ParseConditionOperand(char **tokens, char** operand,
                          int current_cursor, int *number_of_operand_tokens)
{
    int cursor = current_cursor;

    /* variable or arithmetic expression */
    if(strcmp(tokens[cursor], "$") == 0)
    {
        cursor += 1; // consume '$'
                      
        /* arithmetic calculation */
        if(strcmp(tokens[cursor], "((") == 0)
        {
            cursor += 1; // consume "(("
            int i = 0;
            while(strcmp(tokens[cursor], "))") != 0)
            {
                operand[i] = tokens[cursor];
                *number_of_operand_tokens += 1;
                i++;
                cursor++;
            }
            cursor += 1; // consume "))"
        }

        /* just a variable */
        else
        {
            operand[*number_of_operand_tokens] = tokens[cursor];    
            *number_of_operand_tokens += 1;
            cursor += 1; // consume the variable name
        }
    }

    /* just a number */
    else
    {
        /* positive number */
        if(strcmp(tokens[cursor], "-") != 0)
        {
            operand[*number_of_operand_tokens] = tokens[cursor];
            *number_of_operand_tokens += 1;
            cursor += 1; // consume the number
        }
    
        /* negative number */
        else
        {
            operand[*number_of_operand_tokens] = tokens[cursor];
            int len = strlen(tokens[cursor+1]);
            strncat(operand[*number_of_operand_tokens], tokens[cursor+1], len);
            *number_of_operand_tokens += 1;
            cursor += 2; // consume '-' and number
        }
    }

    return cursor;
}

void FreeTree(ast_node_t *node)
{
    if(node->type == ROOT)
    {
        for(int i=0; i<(node->number_of_children); i++)
            FreeTree(node->children[i]);

        free(node->children);
        free(node);

    }
    else if(node->type == BINARY_OPERATION)
    {
        binary_operator_node_t *free_node;
        free_node = (binary_operator_node_t*)node;
        free(free_node->operation);

        FreeTree(free_node->left);
        FreeTree(free_node->right);
    }
    else if(node->type == COMMAND)
    {
        /*
        the memory spaces for tokens are share with the tokernizer. 
        So, There is no need to free the command name and args.
        */
        command_node_t *free_node;
        free_node = (command_node_t*)node;

        free(free_node->args);
        free(free_node);
    }
    else if(node->type == IF)
    {
        if_node_t *if_node;
        if_node = (if_node_t*)node;

        FreeTree((ast_node_t*)if_node->condition);
        FreeTree(if_node->process);

        free(if_node);

    }
    else if(node->type == WHILE)
    {
        while_node_t *while_node;
        while_node = (while_node_t*)node;

        FreeTree((ast_node_t*)while_node->condition);
        FreeTree(while_node->process);

        free(while_node);
    }
    else if(node->type == CONDITION)
    {
        condition_node_t *condition_node;
        condition_node = (condition_node_t*)node;
        free(condition_node);
    }
}

