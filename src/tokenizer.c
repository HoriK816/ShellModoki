#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "tokenizer.h"


char** TokenizeOneLine(char *command_line)
{
    char **tokens = (char **)malloc(sizeof(char*) * MAX_TOKENS);
    if(tokens == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }
    else
    {
        // initialize to NULL  to make it easier to counts the elments.
        for(int i=0;i<MAX_TOKENS;i++)
            tokens[i] = NULL;
    }

    int token_counter = 0;
    int cursor = 0;

    // add 1 to line_len bacause the result of strlen doesn't 
    // include teminatating NULL character.
    int line_len = strlen(command_line)+1;

    for(int i=0;i<line_len;i++)
    {
        // have to check command_line[line_len] to record last word.
        char cc = command_line[i];
        char next_cc = command_line[i+1];

        if(cc == ' ' || cc == '\0')
        {
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

        }
        else if(cc == '&' && next_cc == '&')
        {
            // call AddWordToTokenList to add word before '&&' separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddDoubleAmpersandToTokenList(tokens, command_line,
                                                    cursor, i, &token_counter);
        }
        else if(cc == '|' && next_cc == '|')
        {
            // call AddWordToTokenList to add word before '||' separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddDoublePipeToTokenList(tokens, command_line,
                                              cursor, i, &token_counter);
        }
        else if(cc == ';')
        {
            // call AddWordToTokenList to add word before ';' separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddSemicolonToTokenList(tokens, command_line,
                                             cursor, i, &token_counter);
        }
        else if(cc == '=')
        {
            // call AddWrodToTokenList to add word before '=' separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddEqualToTokenList(tokens, command_line,
                                         cursor, i, &token_counter);
        }
        else if(cc == '(' && next_cc == '(')
        {
            // call AddWrodToTokenList to add word before "((" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddDoubleOpenParenToTokenList(tokens, command_line,
                                                   cursor, i, &token_counter);
        }
        else if(cc == ')' && next_cc == ')')
        {
            // call AddWrodToTokenList to add word before "))" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddDoubleCloseParenToTokenList(tokens, command_line,
                                                    cursor, i, &token_counter);
        }
        else if(cc == '+') 
        {
            // call AddWrodToTokenList to add word before "+" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddPlusToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);
        }
        else if(cc == '-')
        {
            // call AddWrodToTokenList to add word before "-" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddHyphenToTokenList(tokens, command_line,
                                          cursor, i, &token_counter);
        }
        else if(cc == '*')
        {
            // call AddWrodToTokenList to add word before "*" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddAsteriskToTokenList(tokens, command_line,
                                            cursor, i, &token_counter);
        }
        else if(cc == '/')
        {
            // call AddWrodToTokenList to add word before "+" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddSlashToTokenList(tokens, command_line,
                                         cursor, i, &token_counter);
        }
        else if(cc == '%')
        {
            // call AddWrodToTokenList to add word before "+" separator.
            cursor = AddWordToTokenList(tokens, command_line,
                                        cursor, i, &token_counter);

            cursor = AddPercentToTokenList(tokens, command_line,
                                           cursor, i, &token_counter);
        }
    }
    return tokens;
}

int AddWordToTokenList(char **tokens, char *command_line, int cursor,
                       int current_posision, int *token_counter)
{
    int size = current_posision - cursor;

    if(size != 0){
        // when size is 0, it's just a space.
        *token_counter += 1;

        tokens[*token_counter-1] = (char *)malloc(sizeof(char)*100);
        if(tokens[*token_counter-1] == NULL){
            fprintf(stderr, "could not allocate sufficient memory for token");
            exit(EXIT_FAILURE);
        }

        memcpy(tokens[*token_counter-1], &command_line[cursor], size);
        tokens[*token_counter-1][size] = '\0';
    }

    // must increment it not to be included the space character 
    // in the next token.
    cursor = current_posision+1;
    return cursor;
}

int AddSemicolonToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator ;
    *token_counter += 1;

    // need 2 bytes. ';' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL){
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1],";");

    // must increment it not to be included the ';'
    // in the next token.
    cursor = current_posision+1;
    return cursor;

}

int AddDoubleAmpersandToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator &&
    *token_counter += 1;

    // need 3 bytes. '&' '&' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*3);
    if(tokens[*token_counter-1] == NULL){
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1],"&&");

    // must increment it not to be included the '&&'
    // in the next token.
    cursor = current_posision+2;
    return cursor;
}

int AddDoublePipeToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{

    // separator ||
    *token_counter += 1;

    // need 3 bytes. '|' '|' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*3);
    if(tokens[*token_counter-1] == NULL){
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1],"||");

    // must increment it not to be included the '||'
    // in the next token.
    cursor = current_posision+2;
    return cursor;

}

int AddEqualToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator ||
    *token_counter += 1;

    // need 2 bytes. '=' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1],"=");

    // must increment it not to be included the '||'
    // in the next token.
    cursor = current_posision+1;
    return cursor;

}

int AddDoubleOpenParenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // open paren (( 
    *token_counter += 1;

    // need 3 bytes. '(' '(', '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*3);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "((");

    // must increment it not to be included the "(("
    // in the next token.
    cursor = current_posision + 2;
    return cursor;
}

int AddDoubleCloseParenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // open paren (( 
    *token_counter += 1;

    // need 3 bytes. ')' ')', '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*3);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "))");

    // must increment it not to be included the "))"
    // in the next token.
    cursor = current_posision + 2;
    return cursor;
}

int AddPlusToTokenList(char **tokens, char *command_line, int cursor,
                       int current_posision, int *token_counter)
{
    // open paren + 
    *token_counter += 1;

    // need 2 bytes. '+', '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "+");

    // must increment it not to be included the "+"
    // in the next token.
    cursor = current_posision + 1;
    return cursor;
}

int AddHyphenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator - 
    *token_counter += 1;

    // need 2 bytes. '-' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "-");

    // must increment it not to be included the '-'
    // in the next token.
    cursor = current_posision + 1;
    return cursor;

}

int AddAsteriskToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator * 
    *token_counter += 1;

    // need 2 bytes. '*' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "*");

    // must increment it not to be included the '*'
    // in the next token.
    cursor = current_posision + 1;
    return cursor;

}

int AddSlashToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator / 
    *token_counter += 1;

    // need 2 bytes. '/' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "/");

    // must increment it not to be included the '/'
    // in the next token.
    cursor = current_posision + 1;
    return cursor;
}

int AddPercentToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter)
{
    // separator % 
    *token_counter += 1;

    // need 2 bytes. '%' '\0'
    tokens[*token_counter-1] = (char *)malloc(sizeof(char)*2);
    if(tokens[*token_counter-1] == NULL)
    {
        fprintf(stderr, "could not allocate sufficient memory for token");
        exit(EXIT_FAILURE);
    }

    sprintf(tokens[*token_counter-1], "%%");

    // must increment it not to be included the '%'
    // in the next token.
    cursor = current_posision + 1;
    return cursor;
}

int CountTokens(char **tokens)
{
    int number_of_tokens = 0;
    while(tokens[number_of_tokens] != NULL)
        number_of_tokens++;
    return number_of_tokens;
}

