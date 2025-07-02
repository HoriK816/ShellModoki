#pragma once

#include "symbol_table.h"

// parser
int CountTokens(char **tokens);

//ast_node_t* BuildParseTree(char **tokens, int number_of_tokens,
                           //symbol_table_t *symbol_table);
int BuildParseTree(char **tokens, ast_node_t *node,
                   int current_cursor, int number_of_tokens,
                   symbol_table_t *symbol_table);

enum read_mode DecideNextMode(char **tokens, int cursor, int number_of_tokens);
void FreeTree(ast_node_t *node);

int ParseCommand(char **tokens, ast_node_t *node,
                 int current_cursor, int number_of_tokens,
                 symbol_table_t *symbol_table);

int ParseAND(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens,
             symbol_table_t *symbol_table);

int ParseOR(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens,
             symbol_table_t *symbol_table);

int ParseVaribleDifinition(char **tokens, ast_node_t *node,
             int current_cursor, int number_of_tokens);

void ExtractVariable(char **tokens, int current_cursor,
                     symbol_table_t *symbol_table);

int ParseIF(char **tokens, ast_node_t *node, 
            int current_cursor, int number_of_tokens,
            symbol_table_t *symbol_table);

