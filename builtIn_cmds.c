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
#include <signal.h>

/*Prints exit status or terminating signal of the last foreground process
@param
@returns none
*/
void builtIn_status(struct smallsh_shell *smallsh){
    if(smallsh->status < 2) printf("exit value %d\n", smallsh->status);
    //Status above 1 is a signal status
    else printf("terminated by signal %d\n", smallsh->status);
    fflush(stdout);
};

/*Changes directory of smallsh
@param the user input command struct
@returns none
*/
void builtIn_cd(struct userCommand *cmdStruct){
    //No args provided, changes directory to Home path
    if(cmdStruct->numArgs == 0){
        chdir(getenv("HOME"));
    }
    //else attempt to change directory to first arg
    else{
        if(chdir(cmdStruct->args[0]) != 0){
            perror(cmdStruct->args[0]);
        }
    }
};

/*
Prepares program to exit, cleans up memory
@params userCommand struct
@returns none
TODO clean memory leak w/ variable expansion
*/
void builtIn_exit(struct userCommand *cmdStruct, struct smallsh_shell *smallsh){
    //kill and free any background processes still running
    if(smallsh->backTracking->numProcess > 0){
        //kill processes and free memory
        struct background_process *currProcess;
        for(int i=0; i < smallsh->backTracking->numProcess; i++){
            currProcess = smallsh->backTracking->head;

            kill(currProcess->backProcess, 15);
            smallsh->backTracking->head = currProcess->next;
            free(currProcess);
        }
    }
    //Free structure
    freeStruct(cmdStruct);
    return;
};

void freeStruct(struct userCommand *cmdStruct){
    free(cmdStruct);
    return;
}