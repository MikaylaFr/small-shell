/*
Author: Mikayla Friend
File: main.c
Program: smallsh
*/
#include <stdio.h> //printf, scanf
#include "string_parsing.h"

int MAX_CHAR = 2048;

int main(){
    //Upper while loop to continue input prompt
    while(1){
        char inputBuffer[MAX_CHAR];
        printf(":");
        fflush(NULL);
        fgets(inputBuffer, MAX_CHAR, stdin);
        //Create struct from user input
        struct userCommand *command = parseInput(inputBuffer);
    }
    return 0;
}