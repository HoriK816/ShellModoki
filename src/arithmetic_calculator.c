#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "arithmetic_calculator.h"

/* return the evaluated value */
int EvaluateArithmeticTree(arithmetic_node_t* node,
                           symbol_table_t *symbol_table)
{
    int result = 0;

    /* It isn't arithmetic expression but a number */
    if(node->priority == NUMBER)
    {
        result = GetNodeValue(node, symbol_table);
        return result;
    }
     
    /* grab values for calculation */
    int left_value;
    int right_value;
    if(node->left_tree->priority == 3 && node->right_tree->priority == 3)
    {
        left_value  = GetNodeValue(node->left_tree, symbol_table);
        right_value = GetNodeValue(node->right_tree, symbol_table);
    }
    else if(node->left_tree->priority == 3)
    {
        left_value  = GetNodeValue(node->left_tree, symbol_table);
        right_value = EvaluateArithmeticTree(node->right_tree, symbol_table);
    }
    else if(node->right_tree->priority == 3)
    {
        left_value  = EvaluateArithmeticTree(node->left_tree, symbol_table);
        right_value = GetNodeValue(node->right_tree, symbol_table);
    }
    else
    {
        left_value  = EvaluateArithmeticTree(node->left_tree, symbol_table);
        right_value = EvaluateArithmeticTree(node->right_tree, symbol_table);
    }

    /* execute caulculation */
    if(strcmp(node->operation, "+") == 0)
        result = left_value + right_value;

    else if(strcmp(node->operation, "-") == 0)
        result = left_value - right_value;

    else if(strcmp(node->operation, "*") == 0)
        result = left_value * right_value;

    else if(strcmp(node->operation, "/") == 0)
        result = left_value / right_value;

    else if(strcmp(node->operation, "%") == 0)
        result = left_value % right_value;

    return result;
}

/* extract value from nodes */
int GetNodeValue(arithmetic_node_t* node, symbol_table_t* symbol_table)
{
    int value;
    if(node->is_variable)
    {
        /* seek the corresponded value  from symbol_table */
        for(int i=0; i<symbol_table->number_of_records;i++)
        {
            if(strcmp(node->operation, symbol_table->symbol_name[i]) == 0)
                value = *symbol_table->values[i];
        }
    }
    else
        value = node->value;  // just a value 

    return value;
}


arithmetic_node_t* BuildArithmeticTree(char **tokens, int arithmetic_cursor, 
                        arithmetic_node_t* root, int number_of_tokens)
{
    int cursor = arithmetic_cursor;

    while(cursor < number_of_tokens)
    {
        /* operation */
        if((strtok(tokens[cursor], "+") == 0)
                || (strtok(tokens[cursor], "-") == 0)
                || (strtok(tokens[cursor], "*") == 0)
                || (strtok(tokens[cursor], "/") == 0)
                || (strtok(tokens[cursor], "%") == 0))
        {
            root = InsertOperator(tokens, &cursor,
                                    root, number_of_tokens);
        }
        /* value */
        else
        {
            root = InsertOperand(tokens, &cursor, 
                                   root, number_of_tokens);                 
        }
    }

    return root;
}

/* create an new operator node and insert it into the tree */
arithmetic_node_t* InsertOperator(char **tokens, int *current_cursor, 
                                  arithmetic_node_t* root,
                                  int number_of_tokens)
{
    int *cursor = current_cursor;

    /* prepare new operator node */
    arithmetic_node_t* new_node = CreateArithmeticNode();
    new_node->operation         = tokens[*cursor];

    /* set the priority */
    if(strtok(tokens[*cursor], "+") == 0)
        new_node -> priority = PLUS;
    else if(strtok(tokens[*cursor], "-") == 0)
        new_node -> priority = MINUS;
    else if(strtok(tokens[*cursor], "*") == 0)
        new_node -> priority = MULTIPLY;
    else if(strtok(tokens[*cursor], "/") == 0)
        new_node -> priority = DIVIDE;
    else if(strtok(tokens[*cursor], "%") == 0)
        new_node -> priority = MOD;

    *cursor += 1; // consume an operator

    /* insert the operator to the tree*/
    if(new_node->priority <= root->priority)
    {
        arithmetic_node_t* temp_node;
        new_node->left_tree 
            = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));

        /* replace */
        temp_node           = root;
        root                = new_node;
        new_node->left_tree = temp_node;
    }
    else
    {
        arithmetic_node_t* temp_node;
        new_node->left_tree 
            = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));

        /* replace */
        temp_node           = root->right_tree;
        root->right_tree    = new_node;
        new_node->left_tree = temp_node;
    }
    
    return root;
}

/* create an new operand node and insert it into the tree */
arithmetic_node_t* InsertOperand(char **tokens, int *current_cursor, 
                                 arithmetic_node_t* root, int number_of_tokens)
{
    int *cursor = current_cursor;

    /* prepare new operand node */
    arithmetic_node_t* new_node = CreateArithmeticNode();

    /* set the value */
    if(isdigit(tokens[*cursor][0]))
        new_node->value        = atoi(tokens[*cursor]);
    else
    {
        new_node-> operation   = tokens[*cursor]; // set the variable name
        new_node-> is_variable = true;
    }
    new_node->priority = NUMBER;

    *cursor += 1; // consume an operand 

    /* insert the operand to the tree */
    if(root == NULL)                
        root = new_node;
    else
    {
        arithmetic_node_t *insert_spot = root;
        while(true)
        {
            if(insert_spot->right_tree == NULL)
                break;
            else
                insert_spot = insert_spot->right_tree;
        }

        insert_spot->right_tree 
            = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));
        insert_spot->right_tree = new_node;
    }
    return root;
}

/* create an new node */
arithmetic_node_t* CreateArithmeticNode()
{
    arithmetic_node_t* node; 
    node               = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));

    /* initialization */
    node -> value       = 0;
    node -> is_variable = false;
    node -> operation   = (char*)malloc(sizeof(char)*10);
    node -> left_tree   = NULL;
    node -> right_tree  = NULL;

    return node;
}

/* dump the tree for debugging */
void DumpArithmeticTree(arithmetic_node_t* node, int level, 
                        symbol_table_t* symbol_table)
{
    if(3 <= node->priority)
        return;

    for(int i=0; i<level; i++)
        printf("\t");
    printf("%s\n", node->operation);

    if(node->right_tree != NULL)
    {
        if(node->right_tree->priority == 3)
        {
            for(int i=0; i<level;i++)
                printf("\t");
            for(int i=0; i<4;i++)
                printf("-");
            printf("value %d\n", GetNodeValue(node->right_tree, symbol_table));
        }
    }

    if(node->left_tree != NULL)
    {
        if(node->left_tree->priority == 3)
        {
            for(int i=0; i<level;i++)
                printf("\t");
            for(int i=0; i<4;i++)
                printf("-");
            printf("value %d\n", GetNodeValue(node->left_tree, symbol_table));
        }
    }

    level++;
    if(node->right_tree != NULL)
        DumpArithmeticTree(node->right_tree, level, symbol_table);
    if(node->left_tree != NULL)
        DumpArithmeticTree(node->left_tree,  level, symbol_table);
}
