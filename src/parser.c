#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

#include "parser.h"

ast_node_t* BuildParseTree(char **tokens, int number_of_tokens,
                           symbol_table_t *symbol_table)
{
    // prepare the root node
    ast_node_t *root;
    root = (ast_node_t*)malloc(sizeof(ast_node_t));
    if(root == NULL)
    {
        fprintf(stderr, "could not sufficient memory for ast_node");
        exit(EXIT_FAILURE);
    }

    root->type = ROOT;
    root->number_of_children = 0;

    // 100 is a tentative value
    root->children = (ast_node_t**)malloc(sizeof(ast_node_t*) * 100);
    if(root->children == NULL)
    {
        fprintf(stderr, "could not sufficient memory for ast_node");
        exit(EXIT_FAILURE);
    }

    enum read_mode mode;
    int cursor = 0;
    while(cursor < number_of_tokens)
    {
        mode = DecideNextMode(tokens, cursor, number_of_tokens);
        switch(mode)
        {
           case PARSE_COMMAND:
               cursor = ParseCommand(tokens, root, cursor, number_of_tokens,
                                     symbol_table);
               break;

           case PARSE_SEMICOLON:
               break;

           case PARSE_AND:
               cursor = ParseAND(tokens, root, cursor, number_of_tokens,
                                 symbol_table); 
               break;

           case PARSE_OR:
                cursor = ParseOR(tokens, root, cursor, number_of_tokens,
                                 symbol_table);
               break;

           case PARSE_VARIABLE_DIFINITION:
               cursor = ParseVaribleDifinition(tokens, root, cursor,
                                               number_of_tokens);
               break;

           case PARSE_VARIABLE:
               ExtractVariable(tokens, cursor, symbol_table);
               break;

        }
    }
    return root;
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
    enum read_command_mode mode = READ_COMMAND_NAME;

    command_node_t *command_node;
    int cursor = current_cursor;
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


        if(mode == READ_COMMAND_NAME)
        {
            // create new command node
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
    binary_operator_node_t *and_node;
    and_node = (binary_operator_node_t*)malloc(sizeof(binary_operator_node_t));
    if(and_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);

    }

    (and_node->node).type = BINARY_OPERATION;

    and_node->operation = (char*)malloc(sizeof(char) * 5);
    if(and_node->operation == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }
    sprintf(and_node->operation, "&&");
    
    and_node->right = (ast_node_t*)malloc(sizeof(ast_node_t));
    if(and_node->right == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for ast node");
        exit(EXIT_FAILURE);
    }

    // left branch
    int last = node->number_of_children-1;
    and_node->left = node->children[last]; 
    node->children[last] = (ast_node_t *)and_node;

    // increment this cursor because ParseAND cost a "&&" tokens.
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
    binary_operator_node_t *and_node;
    and_node = (binary_operator_node_t*)malloc(sizeof(binary_operator_node_t));
    if(and_node == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }

    (and_node->node).type = BINARY_OPERATION;

    and_node->operation = (char*)malloc(sizeof(char) * 5);
    if(and_node->operation == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }
    sprintf(and_node->operation, "||");
    
    and_node->right = (ast_node_t*)malloc(sizeof(ast_node_t));
    if(and_node->right == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memroy for ast node");
        exit(EXIT_FAILURE);
    }

    // left branch
    int last = node->number_of_children-1;
    and_node->left = node->children[last]; 
    node->children[last] = (ast_node_t *)and_node;

    // increment this cursor because ParseAND cost a "&&" tokens.
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

int ParseVaribleDifinition(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens)
{
    int cursor = current_cursor;
    variable_define_node_t *variable_node;
    variable_node = (variable_define_node_t*)malloc(
                                    sizeof(variable_define_node_t));
    if(variable_node == NULL)
    {
        fprintf(stderr, "could not allcate sufficient memory for ast node");
        exit(EXIT_FAILURE);
    }

    (variable_node->node).type = VARIABLE_DIFINITION;
    
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

int ParseCondition(char **tokens, ast_node_t *node, 
             int current_cursor, int number_of_tokens)
{
    int cursor = current_cursor;
    condition_node_t *condition_node 
        = (condition_node_t*)malloc(sizeof(condition_node_t));

    cursor += 1; // [
    
    condition_node->operand1 = atoi(tokens[cursor]);
    
    cursor += 1; // operand 1

    // 10 is a tentative value
    condition_node->operation = (char*)malloc(sizeof(char) * 10); 
    condition_node->operation = tokens[cursor];

    cursor += 1; // operation
                 
    condition_node->operand2 = atoi(tokens[cursor]);
    cursor += 1; // operand 2
                 
    cursor += 1; // ]

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
}

