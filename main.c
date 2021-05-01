/*
Author: Mikayla Friend
File: main.c
Program: smallsh
Resources: //https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c
*/
#include <stdio.h> //printf, scanf
#include <string.h> //string manipulation
#include "string_parsing.h"

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
            continue;
        }
        test_printStruct(userInput);

        //Detect comment, skip user input
        if(userInput->command[0] == '#') continue;
        printf("This shouldnt print after comment\n");
    }
    return 0;
}