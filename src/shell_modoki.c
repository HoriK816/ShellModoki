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


int main(int argc, char** argv)
{
    // symbol table
    symbol_table_t *symbol_table = CreateSymbolTable();

    while(true)
    {
        PrintPrompt();

        char **command_tokens;
        command_tokens = (char **)malloc(sizeof(char*) * 100);
        int number_of_tokens = 0;

        bool is_ended;
        do
        {
            // get user input
            char* user_input_line = GetUserInputLine();
            RecordCommandHistory(user_input_line);

            // tokenize
            char** tokens_in_line        = TokenizeOneLine(user_input_line);
            int number_of_tokens_in_line = CountTokens(tokens_in_line);

            for(int i=0; i<number_of_tokens_in_line; i++) 
            {
                command_tokens[number_of_tokens] = tokens_in_line[i];     
                number_of_tokens++;
            }

            // for debugging
            DumpTokenizeResult(command_tokens, number_of_tokens);

            is_ended = IsPromptEnded(command_tokens, number_of_tokens);
            
            // for debugging
            if(is_ended)
                printf("user's input is ended\n");
            else
                printf("user's input is not ended. continue ... \n");

        }
        while(!is_ended);

        // free tokens
        for(int i=0; i<number_of_tokens; i++)
            free(command_tokens[i]);
        free(command_tokens);

        /*ast_node_t *root;*/
        /*root = BuildParseTree(command_tokens, number_of_tokens, symbol_table);*/

        /*// for debugging*/
        /*DumpParseTree(root, 0);*/

        /*ExecTree(root, symbol_table);*/
        
        // for debugging
        //DumpSymbolTable(symbol_table);

        /*FreeTree(root);*/
    }
    FreeSymbolTable(symbol_table);
    return 0;
}

char* GetUserInputLine(void)
{
    char *user_input;
    // user can use 10 commands at most
    user_input = (char*)malloc(sizeof(char) * 100); 
    if(user_input == NULL)
    {
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

bool IsPromptEnded(char **tokens, int number_of_tokens)
{
    // TODO: There are more indicators in sh that also should be supported.
    enum indicator
    {
        WHILE, IF, 
    };

    enum indicator pending_indicator_stack[100];
    int stack_pointer = 0;

    for(int i=0; i<number_of_tokens; i++)
    {
        // push "while" 
        if(strcmp(tokens[i], "while") == 0)
        {
            pending_indicator_stack[stack_pointer] = WHILE;
            stack_pointer++;
        }

        // push "if"
        if(strcmp(tokens[i], "if") == 0)
        {
            pending_indicator_stack[stack_pointer] = IF;
            stack_pointer++;
        }

        // pop "if"
        if(strcmp(tokens[i], "fi") == 0)
        {
            if(pending_indicator_stack[stack_pointer-1] == IF)
                stack_pointer--;
            else
            {
                printf("Syntax Error!!");
                return false;
            }
        }

        if(strcmp(tokens[i], "done") == 0)
        {
            if(pending_indicator_stack[stack_pointer-1] == WHILE)
                stack_pointer--;
            else
            {
                printf("Syntax Error!!");
                return false;
            }
        }
    }

    if(stack_pointer == 0)
        return true;
    else
        // The prompt is not ended because 
        // there are remained pending indicators.
        return false;
}


