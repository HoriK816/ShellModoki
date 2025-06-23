#pragma once 
#define MAX_TOKENS 50

// tokenizer
char** TokenizeCommand(char *command);
char** TokenizeOneLine(char *command_line);

int AddWordToTokenList(char **tokens, char *command_line, int cursor,
                       int current_posision, int *token_counter);

int AddSemicolonToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddDoubleAmpersandToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddDoublePipeToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddEqualToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int CountTokens(char **tokens);

// for debugging
void DumpTokenizeResult(char **tokens, int number_of_tokens);
