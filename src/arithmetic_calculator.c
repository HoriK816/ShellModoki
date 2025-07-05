#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "arithmetic_calculator.h"


int main(void)
{
    char ** tokens = (char **)malloc(sizeof(char*)*100);
    for(int i=0; i<100; i++)
        tokens[i] = (char*)malloc(sizeof(char)*10);

    // test case
    // 1 + 2 - 3 * 4 / 2 
    // 1 + 2 - 12 / 2
    // 1 + 2 - 6
    // 3 - 6
    // -3
    tokens[0] = "1";
    tokens[1] = "+";
    tokens[2] = "2";
    tokens[3] = "-";
    tokens[4] = "3";
    tokens[5] = "*";
    tokens[6] = "4";
    tokens[7] = "/";
    tokens[8] = "2";

    /*tokens[0] = "1";*/
    /*tokens[1] = "+";*/
    /*tokens[2] = "1";*/
    /*tokens[3] = "+";*/
    /*tokens[4] = "1";*/
    /*tokens[5] = "+";*/
    /*tokens[6] = "1";*/
    /*tokens[7] = "+";*/
    /*tokens[8] = "1";*/

    int cursor = 0;
    int number_of_tokens = 9;
    arithmetic_node_t* root = NULL;
    root = BuildArithmeticTree(tokens, cursor, root, number_of_tokens);

    DumpArithmeticTree(root, 0);

    printf("reuslt : %d\n", EvaluateArithmeticTree(root));

    return 0;
}

int EvaluateArithmeticTree(arithmetic_node_t* node)
{
    int result = 0;
    int left_value;
    int right_value;

    if(node->left_tree->priority == 3 && node->right_tree->priority == 3)
    {
        left_value  = node->left_tree->value; 
        right_value = node->right_tree->value;
    }
    else if(node->left_tree->priority == 3)
    {
        left_value  = node->left_tree->value; 
        right_value = EvaluateArithmeticTree(node->right_tree);
    }
    else if(node->right_tree->priority == 3)
    {
        left_value  = EvaluateArithmeticTree(node->left_tree);
        right_value = node->right_tree->value;
    }
    else
    {
        left_value  = EvaluateArithmeticTree(node->left_tree);
        right_value = EvaluateArithmeticTree(node->right_tree);
    }

    if(strtok(node->operation, "+") == 0)
    {
        result = left_value + right_value;
    }
    else if(strtok(node->operation, "-") == 0)
    {
        result = left_value - right_value;
    }
    else if(strtok(node->operation, "*") == 0)
    {
        result = left_value * right_value;
    }
    else if(strtok(node->operation, "/") == 0)
    {
        result = left_value / right_value;
    }
    else if(strtok(node->operation, "%") == 0)
    {
        result = left_value % right_value;
    }

    return result;

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

arithmetic_node_t* InsertOperator(char **tokens, int *current_cursor, 
                    arithmetic_node_t* root, int number_of_tokens)
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


arithmetic_node_t* InsertOperand(char **tokens, int *current_cursor, 
                                 arithmetic_node_t* root, int number_of_tokens)
{
    int *cursor = current_cursor;

    /* prepare new operand node */
    arithmetic_node_t* new_node = CreateArithmeticNode();
    new_node->value             = atoi(tokens[*cursor]);

    new_node->priority = NUMBER;

    *cursor += 1; // consume an operand 

    if(root == NULL)                
    {
        root = new_node;
    }
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

        insert_spot->right_tree = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));
        insert_spot->right_tree = new_node;
    }

    return root;
}

arithmetic_node_t* CreateArithmeticNode()
{
    arithmetic_node_t* node; 
    node               = (arithmetic_node_t*)malloc(sizeof(arithmetic_node_t));

    /* initialization */
    node -> value      = 0;
    node -> operation  = (char*)malloc(sizeof(char)*10);
    node -> left_tree  = NULL;
    node -> right_tree = NULL;

    return node;
}


void DumpArithmeticTree(arithmetic_node_t* node, int level)
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
            printf("value %d\n", node->right_tree->value);
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
            printf("value %d\n", node->left_tree->value);
        }
    }

    level++;
    if(node->right_tree != NULL)
        DumpArithmeticTree(node->right_tree, level);
    if(node->left_tree != NULL)
        DumpArithmeticTree(node->left_tree,  level);

}
