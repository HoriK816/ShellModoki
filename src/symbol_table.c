#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include "symbol_table.h"

symbol_table_t* CreateSymbolTable(void)
{
    symbol_table_t *symbol_table;
    symbol_table = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    if(symbol_table == NULL)
    {
        fprintf(stderr, 
                "could not allocate sufficient memory for symbol_table");
        exit(EXIT_FAILURE);
    }
    InitializeSymbolTable(symbol_table);
    return symbol_table;
}

void InitializeSymbolTable(symbol_table_t *symbol_table)
{
    symbol_table->number_of_records = 0;
    symbol_table->symbol_name = (char **)malloc(sizeof(char *) * 1024);
    if(symbol_table->symbol_name == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }

    symbol_table->values = (int **)malloc(sizeof(int *) * 1024);
    if(symbol_table->values == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }
}

void FreeSymbolTable(symbol_table_t *symbol_table)
{
    for(int i=0; i<symbol_table->number_of_records; i++)
    {
        free(symbol_table->symbol_name[i]);
        free(symbol_table->values[i]);
    }
    free(symbol_table->symbol_name);
    free(symbol_table->values);

    free(symbol_table);
}

void StoreVaribale(variable_define_node_t *variable_node,
                   symbol_table_t* symbol_table)
{
    char *name = variable_node->variable_name;

    for(int i=0; i<symbol_table->number_of_records; i++)
    {
        if(strcmp(name, symbol_table->symbol_name[i]) == 0)
        {
            *symbol_table->values[i] = variable_node->value;
            return;
        }
    }

    // register new variable to the table
    int last = symbol_table->number_of_records;
    symbol_table->symbol_name[last] =  (char *)malloc(sizeof(char)*50);
    if(symbol_table->symbol_name[last] == NULL)
    {
        fprintf(stderr,
                "could not allocate for sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }

    strcpy(symbol_table->symbol_name[last], name);

    symbol_table->values[last] = (int *)malloc(sizeof(int));
    if(symbol_table->values[last] == NULL)
    {
        fprintf(stderr,
                "could not allocate sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }
    *symbol_table->values[last] = variable_node->value;
    symbol_table->number_of_records++;
}
