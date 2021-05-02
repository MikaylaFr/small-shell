/*
Author: Mikayla Friend
File: builtIn_cmds.c
Program: smallsh
*/
#include "all_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h> //chdir

/*Changes directory of smallsh
@param the user input command struct
@returns none
*/
void builtIn_cd(struct userCommand *cmdStruct){
    //No args provided, changes directory to Home path
    if(cmdStruct->numArgs == 0){
        chdir(getenv("HOME"));
        char currPath[256];
        printf("Current directory: %s", getcwd(currPath, 255));
    }
    //else attempt to change directory to first arg
    else{
        if(chdir(cmdStruct->args[0]) != 0){
            perror("chdir failed");
        }
        char currPath[256];
        printf("Current directory: %s", getcwd(currPath, 255));
    }
};

/*
Prepares program to exit, cleans up memory
@params userCommand struct
@returns none
TODO clean memory leak w/ variable expansion
*/
void builtIn_exit(struct userCommand *cmdStruct){
    //Free structure
    freeStruct(cmdStruct);
    return;
};

void freeStruct(struct userCommand *cmdStruct){
    free(cmdStruct);
    return;
}