#Source: CS261 Assignments
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -g3

all : smallsh

smallsh: main.o string_parsing.o builtIn_cmds.o process_management.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c

string_parsing.o : string_parsing.c

builtIn_cmds.o : builtIn_cmds.c

process_management.o : process_management.c

memTest :
	valgrind --tool=memcheck --leak-check=yes smallsh

clean :
	-rm *.o
	-rm smallsh