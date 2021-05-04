/*
Author: Mikayla Friend
File: main.c
Program: smallsh
Resources: //https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c
        Signals explorations
*/
#include <stdio.h> //printf, scanf
#include <string.h> //string manipulation
#include <signal.h>
#include "all_header.h"
#include <stdlib.h>
#include <unistd.h>

int MAX_CHAR = 2048;
int FOREGROUND_MODE = 0;
int TRACK_CHANGE = 0;

//Handler for SIGTSTP
void handle_SIGTSTP(int signo){
    //Enter foreground mode
    if(FOREGROUND_MODE == 0){
        FOREGROUND_MODE = 1;
        char *message = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, message, strlen(message));
    }
    //Exiting foreground mode
    else{
        FOREGROUND_MODE = 0;
        char *message = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, message, strlen(message));
    }
    fflush(stdout);
    TRACK_CHANGE++;
};

int main(){
    //Initialize sigint structure
    struct sigaction SIGINT_action = {{0}};
    //Set handler to ignore
    SIGINT_action.sa_handler = SIG_IGN;
    
    //Register handler
    sigaction(SIGINT, &SIGINT_action, NULL);

    //Initialize SIGTSTP structure
    struct sigaction SIGTSTP_action = {{0}};
    //Set handler
    SIGTSTP_action.sa_handler = handle_SIGTSTP;
    //block all catchable signals
    sigfillset(&SIGTSTP_action.sa_mask);
    //Restart function if interupted
    SIGTSTP_action.sa_flags = SA_RESTART;
    //Register handler
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);

    //Struct to keep track of smallsh values and background processes
    struct background_tracking backgroundTracking;
    backgroundTracking.head = NULL;
    backgroundTracking.tail = NULL;
    backgroundTracking.numProcess = 0;
    
    //Initialize smallsh values
    struct smallsh_shell *smallsh, smallshStruct;
    smallsh = &smallshStruct;
    smallsh->SIGINT_action = &SIGINT_action;
    smallsh->SIGTSTP_action = &SIGTSTP_action;
    smallsh->status = 0;
    smallsh->modeChange = 0;
    smallsh->backTracking = &backgroundTracking;

    //Upper while loop to continue input prompt
    while(1){
        //Get user input
        char inputBuffer[MAX_CHAR];
        printf(":");
        fflush(stdout);
        fgets(inputBuffer, MAX_CHAR, stdin);
        
        
        //Detect empty line, skip user input
        if(strlen(inputBuffer) <= 1) continue;

        //Null the newline character
        inputBuffer[strlen(inputBuffer)-1] = 0;

        //Create struct from user input
        struct userCommand *userInput = parseInput(inputBuffer);
        //Error was detected in parsing
        if(userInput->invalid == 1){
            printf("Invalid entry\n");
            fflush(stdout);
            freeStruct(userInput);
            continue;
        }
        //Detect comment, skip user input
        if(userInput->command[0] == '#'){
            freeStruct(userInput);
            continue;
        };

        //test_printStruct(userInput);

        //Built in commands
        char exit[5] = {"exit"};
        char cd[3] = {"cd"};
        char statusCmd[7] = {"status"};

        //Exit cmd
        if(strcmp(userInput->command, exit) == 0){
            builtIn_exit(userInput, smallsh);
            break;
        }
        //cd cmd
        else if(strcmp(userInput->command, cd) == 0){
            builtIn_cd(userInput);
        }
        //status cmd
        else if(strcmp(userInput->command, statusCmd) == 0){
            builtIn_status(smallsh);
        }
        //other process
        else{
            //Foreground process
            if(userInput->background == 0 || FOREGROUND_MODE == 1){
                foregroundProcess(userInput, smallsh);
            }
            //Background process
            else{
                backgroundProcess(userInput, smallsh);
            }
        }

        //Check for background processes if finished
        if(smallsh->backTracking->numProcess > 0) checkBackground(smallsh);

        //free command struct
        freeStruct(userInput);
    }
    return 0;
}