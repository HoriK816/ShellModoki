#pragma once 
#define MAX_TOKENS 50

// tokenizer
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

int AddDoubleOpenParenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddDoubleCloseParenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddPlusToTokenList(char **tokens, char *command_line, int cursor,
                       int current_posision, int *token_counter);

int AddHyphenToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddAsteriskToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddSlashToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);

int AddPercentToTokenList(char **tokens, char *command_line, int cursor,
                            int current_posision, int *token_counter);
// for debugging
void DumpTokenizeResult(char **tokens, int number_of_tokens);
