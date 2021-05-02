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
            exit(1);
            break;
        case 0:
            //Child process
            execvp(cmdStruct->command, execArr);
            //if error
            exit(2);
            break;
        default:
            spawnPid = waitpid(spawnPid, &childStatus, 0);
            fflush(void);
            
            return;
    }
    
}