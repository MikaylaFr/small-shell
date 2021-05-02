/*
Author: Mikayla Friend
File: string_parsing.h
Program: smallsh
*/
#ifndef ALL_HEADER
#define ALL_HEADER

struct userCommand{
    char *command;
    char *args[512]; //Array of strings
    int numArgs; //Number of args
    char *inputFile;
    char *outputFile;
    int background;
    int invalid;
    int variableExp;
};

//built in commands
void builtIn_cd(struct userCommand *cmdStruct);
void builtIn_exit(struct userCommand *cmdStruct);
void freeStruct(struct userCommand *cmdStruct);

//String parsing
struct userCommand *parseInput(char *inputBuffer);
char *initToken(char *userInput, int firstCall, char **savePtr, struct userCommand *cmdStruct);

//testing
void test_printStruct(struct userCommand *cmdStruct);

#endif