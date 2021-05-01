#include "string_parsing.h"
#include <string.h> //String manipulation functions
#include <stdio.h> //printf

void test_printStruct(struct userCommand *cmdStruct){
    printf("\nCommand: %s", cmdStruct->command);
    printf("\nNumArgs: %d", cmdStruct->numArgs);
    printf("\nargs: ");
    for(int i=0; i <= cmdStruct->numArgs; i++){
        if(cmdStruct->args[i] == NULL) printf("null");
        else printf("%s ", cmdStruct->args[i]);
    }
    printf("\nInputFile: %s", cmdStruct->inputFile);
    printf("\nOutFile: %s", cmdStruct->outputFile);
    printf("\nBackground: %d\n", cmdStruct->background);
}