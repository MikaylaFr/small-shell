#Source: CS261 Assignments
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -g3

all : smallsh

smallsh: main.o string_parsing.o testing.o builtIn_cmds.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c

string_parsing.o : string_parsing.c

testing.o : testing.c

builtIn_cmds.o: builtIn_cmds.c

memTest :
	valgrind --tool=memcheck --leak-check=yes smallsh

clean :
	-rm *.o
	-rm smallsh