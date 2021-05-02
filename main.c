/*
Author: Mikayla Friend
File: main.c
Program: smallsh
Resources: //https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c
*/
#include <stdio.h> //printf, scanf
#include <string.h> //string manipulation
#include "all_header.h"

int MAX_CHAR = 2048;

int main(){
    //Upper while loop to continue input prompt
    while(1){
        //Get user input
        char inputBuffer[MAX_CHAR];
        printf(":");
        fflush(NULL);
        fgets(inputBuffer, MAX_CHAR, stdin);
        
        //Detect empty line, skip user input
        if(strlen(inputBuffer) == 1) continue;

        //Null the newline character
        inputBuffer[strlen(inputBuffer)-1] = 0;

        //Create struct from user input
        struct userCommand *userInput = parseInput(inputBuffer);
        //Error was detected in parsing
        if(userInput->invalid == 1){
            printf("Invalid entry\n");
            fflush(NULL);
            freeStruct(userInput);
            continue;
        }
        
        //Detect comment, skip user input
        if(userInput->command[0] == '#'){
            freeStruct(userInput);
            continue;
        };

        test_printStruct(userInput);

        //Built in commands
        char exit[5] = {"exit"};
        char cd[3] = {"cd"};
        char statusCmd[7] = {"status"};

        //Exit cmd
        if(strcmp(userInput->command, exit) == 0){
            builtIn_exit(userInput);
            break;
        }
        //cd cmd
        else if(strcmp(userInput->command, cd) == 0){
            builtIn_cd(userInput);
        }
        //status cmd
        else if(strcmp(userInput->command, statusCmd) == 0){
            //TODO
        }
        //other process
        else{
            //TODO
        }

        //free command struct
        freeStruct(userInput);
    }
    return 0;
}