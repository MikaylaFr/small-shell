/*
Author: Mikayla Friend
File: string_parsing.c
Program: smallsh
*/
#include <string.h> //String manipulation functions
#include <stdio.h> //printf
#include <stdlib.h> //calloc
#include <sys/types.h> //pid_t
#include <unistd.h> //getpid
#include "all_header.h"

/*
Creates token from user input, handle variable expansion.
@param string with space deliniated tokens, 1 for first call, 0 for all calls after
    save pointer for strtok_r
@return token, returns NULL when done.
*/
char *initToken(char *userInput, int firstCall, char **savePtr, struct userCommand *cmdStruct){
    char *token = NULL;
    //if first call
    if(firstCall == 1) token = strtok_r(userInput, " ", savePtr);
    //after first call
    else token = strtok_r(NULL, " ", savePtr);

    //No more tokens left in string
    if(token == NULL) return NULL;

    //Find all expansion variables and concatenate
    char *buffer = NULL;
    for(char *subStringPtr = strstr(token, "$$"); subStringPtr != NULL; subStringPtr = strstr(token, "$$")){
        //Get processID
        int processIdInt = getpid();
        char processIdStr[7];

        //Convert to string
        int processIdLen = sprintf(processIdStr, "%d", processIdInt);
        //Error detection
        if(processIdLen < 0){
            return EXIT_FAILURE;
        } 

        //Create buffer
        buffer = calloc(strlen(token) + processIdLen, sizeof(char));
        //Find distance between sub-string ptr and beginning of string to determine how many bytes to copy
        int size = subStringPtr - token;
        //Concatenate up to before expansion
        strncat(buffer, token, size);
        //Concatenate PID
        strncat(buffer, processIdStr, processIdLen);
        //Concatenate rest of string if more
        if(strlen(token) - size > 2) strncat(buffer, subStringPtr+2, strlen(token) - size);

        if(cmdStruct->variableExp > 0){
            free(token);
        }
        //replace token with variable expansion token
        token = buffer;
        cmdStruct->variableExp++;
    }

    return token;
};

/*
Extract tokens from the provided string and puts it in the userCommand
struct.
@param inputBuffer (is the user command)
@returns userCommand struct (with extracted commands & arguements)
*/
struct userCommand *parseInput(char *inputBuffer){
    //Create new struct
    struct userCommand *newCommand = malloc(sizeof(struct userCommand));
    //Initalize
    newCommand->command = NULL;
    newCommand->numArgs = 0;
    newCommand->background = 0;
    newCommand->inputFile = NULL;
    newCommand->outputFile = NULL;
    newCommand->invalid = 0;
    newCommand->variableExp = 0;

    //For strok_r
    char *saveptr;
    //Get first token
    char *token = initToken(inputBuffer, 1, &saveptr, newCommand);
    //First token is command
    newCommand->command = token;
    
    token = initToken(inputBuffer, 0, &saveptr, newCommand);
    //Rest of string are optional entries
    if(token != NULL){
        //Find args until special symbol is recognized
        while(token != NULL && strcmp("<", token) != 0 && strcmp(">", token) != 0 && strcmp("&", token) != 0){
            //Allocate string space
            newCommand->args[newCommand->numArgs] = token;
            newCommand->numArgs++;

            token = initToken(inputBuffer, 0, &saveptr, newCommand);
        }
        //Find all input/output redirection
        while(token != NULL && strcmp("&", token) != 0){
            //If the next token will be input file
            if(strcmp("<", token) == 0){
                token = initToken(inputBuffer, 0, &saveptr, newCommand);
                newCommand->inputFile = token;
            }
            //Next token will be output file
            else if(strcmp(">", token) == 0){
                token = initToken(inputBuffer, 0, &saveptr, newCommand);
                newCommand->outputFile = token;
            }
            //Unrecognized entry
            else {
                newCommand->invalid = 1;
                break;
            }
            token = initToken(inputBuffer, 0, &saveptr, newCommand);
        }
        //Should be at last token
        if(token != NULL && strcmp("&", token) == 0){
            newCommand->background = 1;
        }
        //Entry out of order
        else if(token != NULL) newCommand->invalid = 1;
    }
    
    return newCommand;
};