#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#include "printer.h"

void PrintPrompt()
{
    // get login is not working on WSL
    /*char *user_name;*/
    /*if((user_name = getlogin()) == NULL)*/
    /*{*/
        /*fprintf(stderr, "getlogin() error\n");*/
        /*exit(EXIT_FAILURE);*/
    /*}*/

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
    /*fprintf(stdout, "[%s@ %s %s ]$ ", user_name,*/
                                      /*host_name,*/
                                      /*current_dir);*/

    // prompt for wsl env 
    fprintf(stdout, " @ %s %s]#", host_name, current_dir);
    // free(user_name);
    free(current_dir);
}
