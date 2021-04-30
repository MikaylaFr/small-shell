#Source: CS261 Assignments
CC = gcc
CFLAGS = -g -Wall -std=gnu99 -g3

all : smallsh

smallsh: main.o string_parsing.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.c

string_parsing.o : string_parsing.c string_parsing.h

memTest :
	valgrind --tool=memcheck --leak-check=yes smallsh

clean :
	-rm *.o
	-rm smallsh