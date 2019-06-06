CC=gcc
CFLAGS = -g -std=c99 -w


all: shell.c parse.h 
	${CC} ${CFLAGS} shell.c -o shell
