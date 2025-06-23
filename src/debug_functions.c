#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>


// for debugging tokenizer 
void DumpTokenizeResult(char **tokens, int number_of_tokens)
{
    printf("there are %d tokens.\n", number_of_tokens);
    for(int i=0; i<number_of_tokens;i++){
        printf("%s\n", tokens[i]);
    }
}


// for debugging symbol tables
void DumpSymbolTable(symbol_table_t *symbol_table)
{
    printf("--------------symbol table--------------------\n");
    for(int i=0; i<symbol_table->number_of_records; i++){
        printf("%s : %d\n", symbol_table->symbol_name[i],
                            *symbol_table->values[i]);
    }
   printf("-----------------------------------------------\n"); 
}

