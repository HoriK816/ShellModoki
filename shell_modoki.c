#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

#include "tokenizer.h"
#include "shell_modoki.h"
// #include "parser.h"

void PrintPrompt();
void RecordCommand(char* command);
void PrintHistory();

char* GetUserInput();
char** GetUserCommand(char *buffer);
size_t CountCommand(char **commands);

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table);
pid_t ExecCommand(command_node_t *command_node);
bool ExecBinaryOperator(binary_operator_node_t *binary_node);

void StoreVaribale(variable_define_node_t *variable_node,
                   symbol_table_t* symbol_table);

void InitializeSymbolTable(symbol_table_t *symbol_table);

void DumpSymbolTable(symbol_table_t *symbol_table);

void FreeSymbolTable(symbol_table_t *symbol_table);

int main(int argc, char** argv){

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

        RecordCommand(user_input);

        char** command_tokens;
        command_tokens = TokenizeOneLine(user_input);

        int number_of_tokens = CountTokens(command_tokens);

        // for debugging
        // DumpTokenizeResult(command_tokens,number_of_tokens);

        ast_node_t *root;
        root = BuildParseTree(command_tokens, number_of_tokens, symbol_table);

        // for debugging
        // DumpParseTree(root, 0);

        ExecTree(root, symbol_table);
        
        // for debugging
        //DumpSymbolTable(symbol_table);

        // free
        for(int i=0; i<number_of_tokens; i++)
            free(command_tokens[i]);
        free(command_tokens);

        FreeTree(root);
    }
    FreeSymbolTable(symbol_table);

    return 0;
}

void ExecTree(ast_node_t * node, symbol_table_t *symbol_table)
{

    if(node->type == ROOT){

    }else if(node->type == COMMAND){
        command_node_t *command_node;
        command_node = (command_node_t*)node;
        ExecCommand(command_node);
        return;

    }else if(node->type == BINARY_OPERATION){
        binary_operator_node_t *binary_node;
        binary_node = (binary_operator_node_t*)node;

        ExecBinaryOperator(binary_node);
        return;

    }else if(node->type ==  VARIABLE_DIFINITION){
        variable_define_node_t *variable_node;
        variable_node = (variable_define_node_t*)node;

        StoreVaribale(variable_node, symbol_table);
        return;
    }
    
    for(int i=0;i<node->number_of_children;i++){
        ast_node_t *next_node;
        next_node  = (ast_node_t *)node->children[i];
        ExecTree(next_node, symbol_table);
    }
}

bool ExecBinaryOperator(binary_operator_node_t *binary_node){

    bool result;

    // left branch 
    pid_t status;
    int exit_code;
    if(binary_node->left->type == COMMAND){
        status = ExecCommand((command_node_t*)binary_node->left);

        exit_code = WEXITSTATUS(status);
        if(exit_code == 0){
            if(strcmp(binary_node->operation, "||") == 0)
                return true;
        }else{
            if(strcmp(binary_node->operation, "&&") == 0)
                return false;
        }
    }

    bool is_success;
    if(binary_node->left->type == BINARY_OPERATION){
        is_success = ExecBinaryOperator(
                        (binary_operator_node_t*)binary_node->left); 

        if(is_success){
            if(strcmp(binary_node->operation, "||") == 0)
                return true;
        }else{
            if(strcmp(binary_node->operation, "&&") == 0)
                return false;
        }

    }


    // right branch
    if(binary_node->right->type == COMMAND){
        status = ExecCommand((command_node_t*)binary_node->right);
        exit_code = WEXITSTATUS(status);

        if(exit_code == 0 )
            result = true;
    }

    if(binary_node->right->type == BINARY_OPERATION){
        is_success = ExecBinaryOperator(
                        (binary_operator_node_t*)binary_node->right); 

        if(is_success)
            result = true;

    }

    return result;
}

pid_t ExecCommand(command_node_t *command_node)
{
    pid_t status;
    char *command_pointer[10];
    
    command_pointer[0] = command_node->command;

    for(int i=0;i<(command_node->number_of_args);i++){
        command_pointer[i+1] = (command_node->args)[i];
    }
    command_pointer[command_node->number_of_args+1] = NULL;

    // exit
    if(strcmp(command_pointer[0], "exit") == 0){
        exit(0);
    }

    // history
    if(strcmp(command_pointer[0], "history") == 0){
        PrintHistory();
    }

    // change directory
    if(strcmp(command_pointer[0], "cd") == 0){
        if(command_pointer[1] == NULL){
            chdir(HOME_EXP_ENV);
        }else{
            chdir(command_pointer[1]);
        }
    }

    if(fork() == 0){
        execvp(command_pointer[0], command_pointer);
        fprintf(stderr, "%s is not found \n", command_pointer[0]);
        exit(1);
    }else{
        wait(&status);

    }
    return status;
}

void StoreVaribale(variable_define_node_t *variable_node,
                   symbol_table_t* symbol_table){

    char *name = variable_node->variable_name;

    for(int i=0; i<symbol_table->number_of_records; i++){
        if(strcmp(name, symbol_table->symbol_name[i]) == 0){
            *symbol_table->values[i] = variable_node->value;
            return;
        }
    }

    // register new variable to the table
    int last = symbol_table->number_of_records;
    symbol_table->symbol_name[last] =  (char *)malloc(sizeof(char)*50);
    if(symbol_table->symbol_name[last] == NULL){
        fprintf(stderr,
                "could not allocate for sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }

    strcpy(symbol_table->symbol_name[last], name);

    symbol_table->values[last] = (int *)malloc(sizeof(int));
    if(symbol_table->values[last] == NULL){
        fprintf(stderr,
                "could not allocate sufficient memory for symbol table");
        exit(EXIT_FAILURE);
    }
    *symbol_table->values[last] = variable_node->value;

    symbol_table->number_of_records++;
}

char* GetUserInput(void){

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
                char **ptr = (char**)realloc(commands, sizeof(char*) * word_counter);
                if(ptr == NULL){
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
    if((user_name = getlogin()) == NULL){
        fprintf(stderr, "getlogin() error\n");
        exit(EXIT_FAILURE);
    }

    int rc;
    char host_name[256];
    if((rc = gethostname(host_name, sizeof(host_name))) == -1){
        fprintf(stderr, "gethostname() error\n");
        exit(EXIT_FAILURE);
    }
    
    char *current_dir;
    if((current_dir = getcwd(NULL, 0)) == NULL){
        fprintf(stderr, "getcwd() error\n");
        exit(EXIT_FAILURE);
    }

    // this prompt is not working on WSL
    fprintf(stdout, "[%s@ %s %s ]$ ", user_name,
                                      host_name,
                                      current_dir);

    // prompt for wsl env 
    /*fprintf(stdout, " @ %s %s]#", host_name, current_dir);*/

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

void RecordCommand(char *command)
{
    FILE *history_file = fopen(HISTORY_PATH_EXP_ENV, "a");

    // record command to the history file
    fwrite(command, sizeof(char), strlen(command), history_file);

    // add \n to the end because command doesn't include \n 
    fwrite("\n", sizeof(char), 1, history_file);

    fclose(history_file);
}

void PrintHistory()
{
    FILE *history_file = fopen(HISTORY_PATH_EXP_ENV, "r");

    char cc;
    char next_cc;
    int line_number = 1;

    while((cc = fgetc(history_file)) != EOF){
        // print line number
        if(cc == '\n'){
            if((next_cc = fgetc(history_file)) != EOF){
                // print line number after the line break
                fprintf(stdout, "\n%2d ", line_number);
                line_number++;
                ungetc((int)next_cc, history_file); // for EOF 
            }

        }else if(line_number == 1){ // for the first line
            fprintf(stdout, "%2d %c", line_number, cc);
            line_number++;

        }else{
            fprintf(stdout, "%c", cc);
        }
    }
    fprintf(stdout, "\n");
    fclose(history_file);
}

/*void InitializeSymbolTable(symbol_table_t *symbol_table)*/
/*{*/
    /*symbol_table->number_of_records = 0;*/
    /*symbol_table->symbol_name = (char **)malloc(sizeof(char *) * 1024);*/
    /*if(symbol_table->symbol_name == NULL){*/
        /*fprintf(stderr, "could not allocate sufficient memory for symbol table");*/
        /*exit(EXIT_FAILURE);*/
    /*}*/

    /*symbol_table->values = (int **)malloc(sizeof(int *) * 1024);*/
    /*if(symbol_table->values == NULL){*/
        /*fprintf(stderr, "could not allocate sufficient memory for symbol table");*/
        /*exit(EXIT_FAILURE);*/
    /*}*/
/*}*/

/*void FreeSymbolTable(symbol_table_t *symbol_table)*/
/*{*/
    /*for(int i=0; i<symbol_table->number_of_records; i++)*/
    /*{*/
        /*free(symbol_table->symbol_name[i]);*/
        /*free(symbol_table->values[i]);*/
    /*}*/
    /*free(symbol_table->symbol_name);*/
    /*free(symbol_table->values);*/

    /*free(symbol_table);*/
/*}*/


/*// for debugging*/
/*void DumpSymbolTable(symbol_table_t *symbol_table)*/
/*{*/
    /*printf("--------------symbol table--------------------\n");*/
    /*for(int i=0; i<symbol_table->number_of_records; i++){*/
        /*printf("%s : %d\n", symbol_table->symbol_name[i],*/
                            /**symbol_table->values[i]);*/
    /*}*/
   /*printf("-----------------------------------------------\n"); */
/*}*/

