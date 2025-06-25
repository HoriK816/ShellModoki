#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#include "shell_modoki.h"
#include "symbol_table.h"
#include "tokenizer.h"
#include "parser.h"
#include "history.h"
#include "printer.h"
#include "executor.h"

char* GetUserInput();
size_t CountCommand(char **commands);

int main(int argc, char** argv)
{
    // symbol table
    symbol_table_t *symbol_table;
    symbol_table = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    if(symbol_table == NULL)
    {
        fprintf(stderr, 
                "could not allocate sufficient memory for symbol_table");
        exit(EXIT_FAILURE);
    }

    InitializeSymbolTable(symbol_table);

    while(true)
    {
        PrintPrompt();

        char *user_input;
        user_input = GetUserInput();

        // RecordCommandHistory(user_input);

        char** command_tokens;
        command_tokens = TokenizeOneLine(user_input);

        int number_of_tokens = CountTokens(command_tokens);

        // for debugging
        // DumpTokenizeResult(command_tokens,number_of_tokens);

        ast_node_t *root;
        root = BuildParseTree(command_tokens, number_of_tokens, symbol_table);

        // for debugging
        DumpParseTree(root, 0);

        ExecTree(root, symbol_table);
        
        /*// for debugging*/
        /*//DumpSymbolTable(symbol_table);*/

        /*// free*/
        for(int i=0; i<number_of_tokens; i++)
            free(command_tokens[i]);
        free(command_tokens);

        FreeTree(root);
    }
    FreeSymbolTable(symbol_table);

    return 0;
}

char* GetUserInput(void)
{

    char *user_input;
    // user can use 10 commands at most
    user_input = (char*)malloc(sizeof(char) * 100); 
    if(user_input == NULL){
        fprintf(stderr,
                "could not allocate sufficient memory for user's input");
        exit(EXIT_FAILURE);
    }


    if(fgets(user_input, 100, stdin) == NULL)
        exit(0);

    int len = strlen(user_input);
    // replace from \n to \0
    if(user_input[len-1] == '\n')
        user_input[len-1] = '\0';

    return user_input;
}

size_t CountCommand(char **commands)
{
    size_t counter = 0;
    while(commands[counter] != NULL)
        counter++;
    return counter;
}
