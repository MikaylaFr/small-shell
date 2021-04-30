/*
Author: Mikayla Friend
File: string_parsing.h
Program: smallsh
*/
#ifndef STRING_PARSING
#define STRING_PARSING

struct userCommand{
    char *command;
    char *args[512]; //Array of strings
    int numArgs; //Number of args
    char *inputFile;
    char *outputFile;
    int background;
};

struct userCommand *parseInput(char *inputBuffer);

#endif