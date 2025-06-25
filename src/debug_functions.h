#pragma once 
#include "tokenizer.h"
#include "parser.h"

void DumpTokenizeResult(char **tokens, int number_of_tokens);
void DumpSymbolTable(symbol_table_t *symbol_table);
void DumpParseTree(ast_node_t *node, int level);
