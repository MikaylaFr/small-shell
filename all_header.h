/*
Author: Mikayla Friend
File: string_parsing.h
Program: smallsh
*/
#ifndef ALL_HEADER
#define ALL_HEADER

#include <sys/types.h>
#include <signal.h>

/*Struct to keep track of different parts of the user input*/
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

/*Struct to keep track of smallsh variables*/
struct smallsh_shell{
    int status;
    int modeChange;
    struct background_tracking *backTracking;
    struct sigaction *SIGINT_action;
    struct sigaction *SIGTSTP_action;
};

/*Linked list node with background process information*/
struct background_process{
    pid_t backProcess;
    int backStatus;
    struct background_process *next;
    struct background_process *prev;
};

/*Linked list head*/
struct background_tracking{
    int numProcess;
    struct background_process *head;
    struct background_process *tail;
};

//Process Management
void foregroundProcess(struct userCommand *cmdStruct, struct smallsh_shell *smallsh);
void backgroundProcess(struct userCommand *cmdStruct, struct smallsh_shell *smallsh);
char **execArray(struct userCommand *cmdStruct);
void addToBackList(struct background_process *newProcess, struct smallsh_shell *smallsh);
void checkBackground(struct smallsh_shell *smallsh);
void fileRedirection(struct userCommand *cmdStruct);

//built in commands
void builtIn_cd(struct userCommand *cmdStruct);
void builtIn_exit(struct userCommand *cmdStruct, struct smallsh_shell *smallsh);
void freeStruct(struct userCommand *cmdStruct);
void builtIn_status(struct smallsh_shell *smallsh);

//String parsing
struct userCommand *parseInput(char *inputBuffer);
char *initToken(char *userInput, int firstCall, char **savePtr, struct userCommand *cmdStruct);

//testing
void test_printStruct(struct userCommand *cmdStruct);

#endif