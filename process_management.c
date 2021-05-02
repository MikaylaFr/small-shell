/*
Author: Mikayla Friend
File: process_management.c
Program: smallsh
Resources: OSU process explorations
*/
#include "all_header.h"
#include <sys/wait.h> // for waitpid
#include <stdio.h>    // for printf and perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv, getpid, fork

/* Creates process and executes given command, updates status.
@param the user command parsed into userCommand struct, smallsh_shell struct from calling 
*/
void foregroundProcess(struct userCommand *cmdStruct, struct smallsh_shell *smallsh){
    //Array to be passed to exec
    char **execArr = calloc(cmdStruct->numArgs + 2, sizeof(char*));
    //first string is cmd
    execArr[0] = cmdStruct->command;

    //Copy arguments
    for(int i=1; i <= cmdStruct->numArgs; i++) execArr[i] = cmdStruct->args[i-1];
    
    //Null terminate array
    execArr[cmdStruct->numArgs+1] = NULL;

    //fork new process
    int childStatus;
    pid_t spawnPid = fork();
    switch(spawnPid){
        case -1:
            perror("fork()\n");
            exit(2);
            break;
        case 0:
            //Child process
            execvp(cmdStruct->command, execArr);
            //if error
            perror(cmdStruct->command);
            //flush stdout and stdin
            fflush(NULL);
            exit(1);
            break;
        default:
            spawnPid = waitpid(spawnPid, &childStatus, 0);
            //flush stdout and stdin
            fflush(NULL);
            //find exit status
            //if child exited normally
            if(WIFEXITED(childStatus)){
                //record status
                smallsh->status = WEXITSTATUS(childStatus);
            }
            else{
                //record status
                smallsh->status = WTERMSIG(childStatus);
            }
            //clean memory
            free(execArr);
            return;
    }
    
}