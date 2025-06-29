#include<stdio.h>
#include<string.h>

#include "history.h"

void RecordCommandHistory(char *command)
{
    FILE *history_file = fopen(HISTORY_PATH, "a");

    // record command to the history file
    fwrite(command, sizeof(char), strlen(command), history_file);

    // add \n to the end because command doesn't include \n 
    fwrite("\n", sizeof(char), 1, history_file);

    fclose(history_file);
}

void PrintHistory()
{
    FILE *history_file = fopen(HISTORY_PATH, "r");

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
