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

int main(){
    //Initialize sigint structure
    struct sigaction SIGINT_action;
    //Set handler to ignore
    SIGINT_action.sa_handler = SIG_IGN;
    //Register handler
    sigaction(SIGINT, &SIGINT_action, NULL);
    //Struct to keep track of smallsh values and background processes
    struct background_tracking backgroundTracking;
    backgroundTracking.head = NULL;
    backgroundTracking.tail = NULL;
    backgroundTracking.numProcess = 0;
    
    //Initialize smallsh values
    struct smallsh_shell *smallsh, smallshStruct;
    smallsh = &smallshStruct;
    smallsh->SIGINT_action = &SIGINT_action;
    smallsh->status = 0;                                                                                                                                                                                                                                                                        ;
    smallsh->foregroundMode = 0;
    smallsh->backTracking = &backgroundTracking;

    //Upper while loop to continue input prompt
    while(1){
        //Get user input
        char inputBuffer[MAX_CHAR];
        printf(":");
        fflush(stdout);
        fgets(inputBuffer, MAX_CHAR, stdin);
        fflush(stdin);
        
        //Detect empty line, skip user input
        if(strlen(inputBuffer) == 1) continue;

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
            if(userInput->background == 0){
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