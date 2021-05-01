/*
Author: Mikayla Friend
File: main.c
Program: string_parsing.c
*/
#include <string.h> //String manipulation functions
#include <stdio.h> //printf
#include <stdlib.h> //calloc
#include "string_parsing.h"

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

    //For strok_r
    char *saveptr;
    //Get first token
    char *token = strtok_r(inputBuffer, " ", &saveptr);
    //First token is command
    newCommand->command = calloc(strlen(token)+1, sizeof(char));
    strcpy(newCommand->command, token);
    
    token = strtok_r(NULL, " ", &saveptr);
    //Rest of string are optional entries
    if(token != NULL){
        //Find args until special symbol is recognized
        while(token != NULL && strcmp("<", token) != 0 && strcmp(">", token) != 0 && strcmp("&", token) != 0){
            //Allocate string space
            newCommand->args[newCommand->numArgs] = calloc(strlen(token)+1, sizeof(char));
            //copy token to struct
            strcpy(newCommand->args[newCommand->numArgs], token);
            newCommand->numArgs++;

            token = strtok_r(NULL, " ", &saveptr);
        }
        //Find all input/output redirection
        while(token != NULL && strcmp("&", token) != 0){
            //If the next token will be input file
            if(strcmp("<", token) == 0){
                token = strtok_r(NULL, " ", &saveptr);
                newCommand->inputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCommand->inputFile, token);
            }
            //Next token will be output file
            else if(strcmp(">", token) == 0){
                token = strtok_r(NULL, " ", &saveptr);
                newCommand->outputFile = calloc(strlen(token)+1, sizeof(char));
                strcpy(newCommand->outputFile, token);
            }
            //Unrecognized entry
            else {
                newCommand->invalid = 1;
                break;
            }
            token = strtok_r(NULL, " ", &saveptr);
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