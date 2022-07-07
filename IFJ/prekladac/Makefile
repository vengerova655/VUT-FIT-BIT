#file Makefile
#IFJ20 compiler
#authors: xkucma00
#brief: creates binary for the compiler

SHELL = /bin/sh
.PHONY: all pack test clean valgrind
CC=gcc
CFLAGS=-g -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -Wno-unused-function
LDFLAGS=-g -std=c99 -pedantic -Wall -Wextra
SRC_OBJS=main.o parser.o semgen.o expressions.o scanner.o symtable.o errors.o
PROJ_NAME=ifj20
PACK_NAME=xvenge01.zip

all: $(PROJ_NAME)

pack: clean ../rozdeleni ../rozsireni ../dokumentace.pdf
	zip -j $(PACK_NAME) Makefile *.c *.h ../rozdeleni ../rozsireni ../dokumentace.pdf

test: $(PROJ_NAME) test.in
	./$(PROJ_NAME) <test.in

valgrind: $(PROJ_NAME) test.in
	valgrind ./$(PROJ_NAME) <test.in

clean: 
	rm -f *.o *.out $(PROJ_NAME) $(PACK_NAME)

##

$(PROJ_NAME): $(SRC_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

main.o: main.c

%.o: %.c %.h
