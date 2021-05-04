/*
Author: Mikayla Friend
File: process_management.c
Program: smallsh
Resources: OSU process, exec, and I/O explorations
*/
#include "all_header.h"
#include <sys/wait.h> // for waitpid
#include <sys/types.h>
#include <stdio.h>    // for printf and perror
#include <stdlib.h>   // for exit
#include <unistd.h>   // for execv, getpid, fork
#include <fcntl.h>

/*Handles file redirection
@param cmd struct with parsed command
*/
void fileRedirection(struct userCommand *cmdStruct){
    //Input file redirection
    if(cmdStruct->inputFile != NULL){
        //open input file
        int inputFD = open(cmdStruct->inputFile, O_RDONLY);
        if(inputFD == -1){
            perror(cmdStruct->inputFile);
            exit(1);
        }
        //Redirect input
        int result = dup2(inputFD, 0);
        if(result == -1){
            perror("user input dup2()");
            exit(1);
        }
    }
    //if background process and no user defined redirection input
    else if(cmdStruct->background == 1){
        int inputFD = open("/dev/null", O_RDONLY);
        // redirected to to null
        int result = dup2(inputFD, 0);
        if(result == -1){
            perror("null input dup2()");
            exit(1);
        }        
    };

    //output file redirection
    if(cmdStruct->outputFile != NULL){
        //Open, trunicate, or create file
        int outputFD = open(cmdStruct->outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0664);
        if(outputFD == -1){
            perror(cmdStruct->inputFile);
            exit(1);
        }
        //redirect output
        int result = dup2(outputFD, 1);
        if(result == -1){
            perror("user output dup2()");
            exit(1);
        }
    }
    //if background process and no user defined redirection output
    else if(cmdStruct->background == 1){
        int outputFD = open("/dev/null", O_WRONLY | O_TRUNC);
        // redirected to to null
        int result = dup2(outputFD, 0);
        if(result == -1){
            perror("null output dup2()");
            exit(1);
        }        
    };
}

/*Checks if any background processes have terminated
@params smallsh struct with linked list to background processes
*/
void checkBackground(struct smallsh_shell *smallsh){
    //loop through linked list and check if process has ended
    struct background_process *currProcess = smallsh->backTracking->head;
    while(currProcess != NULL){
        if(waitpid(currProcess->backProcess, &currProcess->backStatus, WNOHANG)){
            //print completion
            printf("background pid %d is done: ", currProcess->backProcess);
            //if child exited normally
            if(WIFEXITED(currProcess->backStatus)){
                //record status
                printf("exit value %d\n", WEXITSTATUS(currProcess->backStatus));
            }
            else{
                //record status
                printf("terminated by signal %d\n", WTERMSIG(currProcess->backStatus));
            }
            //Flush stdout
            fflush(stdout);

            //remove from linked list
            //If only process
            if(smallsh->backTracking->numProcess == 1){
                smallsh->backTracking->head = NULL;
                smallsh->backTracking->tail = NULL;
            }
            else{
                //If last process
                if(currProcess == smallsh->backTracking->tail){
                    smallsh->backTracking->tail = currProcess->prev;
                    smallsh->backTracking->tail->next = NULL;
                    printf("\nLast process\n");
                }
                //first process
                else if(currProcess == smallsh->backTracking->head){
                    smallsh->backTracking->head = currProcess->next;
                    smallsh->backTracking->head->prev = NULL;
                    printf("\nFirst Process\n");
                }
                else{
                    currProcess->next->prev = currProcess->prev;
                    currProcess->prev->next = currProcess->next;
                    printf("\nother\n");
                }
            }
            //Update num
            smallsh->backTracking->numProcess--;
            //free memory
            struct background_process *deleteProcess = currProcess;
            currProcess = currProcess->next;
            free(deleteProcess);
        }
        else currProcess = currProcess->next;
    }
};

/*
Adds the new process to the back of the linked list
@params the new process, smallsh struct with pointer to head and tail of linked list
*/
void addToBackList(struct background_process *newProcess, struct smallsh_shell *smallsh){
    //First process in list
    if(smallsh->backTracking->head == NULL){
        smallsh->backTracking->head = newProcess;
        smallsh->backTracking->tail = newProcess;
        newProcess->next = NULL;
        newProcess->prev = NULL;
        return;
    }
    //Add new process to end of linked list
    smallsh->backTracking->tail->next = newProcess;
    newProcess->prev = smallsh->backTracking->tail;
    newProcess->next = NULL;
    smallsh->backTracking->tail = newProcess;
    return;
};

/*Creates array of strings that will be passed to exec function
@param the user command parsed into userCommand struct
*/
char **execArray(struct userCommand *cmdStruct){
    //Array to be passed to exec
    char **execArr = calloc(cmdStruct->numArgs + 2, sizeof(char*));
    //first string is cmd
    execArr[0] = cmdStruct->command;

    //Copy arguments
    for(int i=1; i <= cmdStruct->numArgs; i++) execArr[i] = cmdStruct->args[i-1];
    
    //Null terminate array
    execArr[cmdStruct->numArgs+1] = NULL;
    return execArr;
};

/* Creates process and executes given command, updates status, parent process does not wait.
@param the user command parsed into userCommand struct, smallsh_shell struct from calling 
*/
void backgroundProcess(struct userCommand *cmdStruct, struct smallsh_shell *smallsh){
    //Get array to be passed to exec
    char **execArr = execArray(cmdStruct);

    //Allocate new background process struct
    struct background_process *newProcess = malloc(sizeof(struct background_process));

    //fork new process
    pid_t spawnPid = fork();
    switch(spawnPid){
        case -1:
            perror("fork()\n");
            exit(1);
            break;
        case 0:
            //Child process
            //Change signal handler for SIGTSTP to ignore;
            smallsh->SIGTSTP_action->sa_handler = SIG_IGN;
            //reregister signal
            sigaction(SIGTSTP, smallsh->SIGTSTP_action, NULL);

            //test for file redirection and use dup2 if necessary
            fileRedirection(cmdStruct);
            //replace process with exec
            execvp(cmdStruct->command, execArr);
            //if error
            perror(cmdStruct->command);
            //flush stdout and stdin
            fflush(stdout);
            exit(1);
            break;
        default:
            //waitpid(spawnPid, smallsh->backTracking->backStatus, WNOHANG);
            //Dont wait for child to complete, add to linked list
            newProcess->backProcess = spawnPid;
            addToBackList(newProcess, smallsh);
            //Update background tracking num
            smallsh->backTracking->numProcess++;

            //Print background Id
            printf("background pid is %d\n", spawnPid);
            //flush stdout and stdin
            fflush(stdout);
            free(execArr);
            return;
    }
};

/* Creates process and executes given command, updates status.
@param the user command parsed into userCommand struct, smallsh_shell struct from calling 
*/
void foregroundProcess(struct userCommand *cmdStruct, struct smallsh_shell *smallsh){
    //Get array to be passed to exec
    char **execArr = execArray(cmdStruct);

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
            //change signal handler for SIGINT
            smallsh->SIGINT_action->sa_handler = SIG_DFL;
            //Reregister signal
            sigaction(SIGINT, smallsh->SIGINT_action, NULL);
            //Change signal handler for SIGTSTP to ignore;
            smallsh->SIGTSTP_action->sa_handler = SIG_IGN;
            //reregister signal
            sigaction(SIGTSTP, smallsh->SIGTSTP_action, NULL);

            //test for file redirection and use dup2 if necessary
            fileRedirection(cmdStruct);

            //replace process with exec
            execvp(cmdStruct->command, execArr);
            //if error
            perror(cmdStruct->command);
            //flush stdout and stdin
            fflush(stdout);
            exit(1);
            break;
        default:
            spawnPid = waitpid(spawnPid, &childStatus, 0);

            //flush stdout and stdin
            fflush(stdout);
            //find exit status
            //if child exited normally
            if(WIFEXITED(childStatus)){
                //record status
                smallsh->status = WEXITSTATUS(childStatus);
            }
            else{
                //record status
                smallsh->status = WTERMSIG(childStatus);
                //print to terminal
                builtIn_status(smallsh);
            }
            //clean memory
            free(execArr);
            return;
    }
}