#pragma once

#define HISTORY_PATH_WSL "/home/horik/develop/ShellModoki/history.log"
#define HISTORY_PATH_MAC "/Users/horik/develop/ShellModoki/history.log"
#define HISTORY_PATH_EXP_ENV "/2024/v24e3026/develop/ShellModoki/history.log"

void RecordCommandHistory(char* command);
void PrintHistory();
