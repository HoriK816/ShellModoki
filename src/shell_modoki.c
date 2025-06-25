#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

#include "tokenizer.h"
#include "shell_modoki.h"
#include "symbol_table.h"
#include "parser.h"
#include "history.h"

void PrintPrompt();

char* GetUserInput();
char** GetUserCommand(char *buffer);
size_t CountCommand(char **commands);

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table);
pid_t ExecCommand(command_node_t *command_node);
bool ExecBinaryOperator(binary_operator_node_t *binary_node);

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


char** GetUserCommand(char *buffer)
{
    char **commands;
    // user can use 10 commands at most
    commands = (char**)malloc(sizeof(char*) * 10); 
    
    if(commands == NULL){
        fprintf(stderr,
                "could not allocate sufficient memory for command buffer");
        exit(EXIT_FAILURE);
    }

    if(fgets(buffer, 80, stdin) == NULL)
        exit(0);

    int len = strlen(buffer);
    // replace from \n to \0
    if(buffer[len-1] == '\n')
        buffer[len-1] = '\0';

    int word_counter = 0;

    if(1<len){
        // separator ; 
        commands[0] = strtok(buffer,";");

        char *next_command;
        while((next_command = strtok(NULL, ";")) != NULL){
            word_counter++;
            
            if(commands[word_counter] == NULL){
                char **ptr = (char**)realloc(commands,
                                             sizeof(char*) * word_counter);
                if(ptr == NULL)
                {
                    fprintf(stderr, "realloc error\n");
                    exit(EXIT_FAILURE);
                }
                commands = ptr;
            }
        }

    }else{
        commands = NULL;
    }
    return commands;
}


void PrintPrompt()
{
    // get login is not working on WSL
    char *user_name;
    if((user_name = getlogin()) == NULL)
    {
        fprintf(stderr, "getlogin() error\n");
        exit(EXIT_FAILURE);
    }

    int rc;
    char host_name[256];
    if((rc = gethostname(host_name, sizeof(host_name))) == -1)
    {
        fprintf(stderr, "gethostname() error\n");
        exit(EXIT_FAILURE);
    }
    
    char *current_dir;
    if((current_dir = getcwd(NULL, 0)) == NULL)
    {
        fprintf(stderr, "getcwd() error\n");
        exit(EXIT_FAILURE);
    }

    // this prompt is not working on WSL
    fprintf(stdout, "[%s@ %s %s ]$ ", user_name,
                                      host_name,
                                      current_dir);

    // prompt for wsl env 
    /* fprintf(stdout, " @ %s %s]#", host_name, current_dir);*/
    // free(user_name);
    free(current_dir);
}

size_t CountCommand(char **commands)
{
    size_t counter = 0;
    while(commands[counter] != NULL)
        counter++;
    return counter;
}
