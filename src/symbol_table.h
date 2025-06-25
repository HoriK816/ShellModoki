#pragma once
#include "shell_modoki.h"

typedef struct{
    char **symbol_name;
    int **values;
    int number_of_records;
}symbol_table_t;

symbol_table_t* CreateSymbolTable(void);
void InitializeSymbolTable(symbol_table_t *symbol_table);
void FreeSymbolTable(symbol_table_t *symbol_table);

void StoreVaribale(variable_define_node_t *variable_node,
                   symbol_table_t* symbol_table);

