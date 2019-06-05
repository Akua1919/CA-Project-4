#ifndef PARSE_H
#define PARSE_H

typedef struct Queue
{
    char** history;
    int loc;
    int size;
} Queue;

typedef struct Cmd
{
    char** command_list;
    char** argument_list;
    int pipe;
    char* input;
    char* output;
    int background;
    int opkind;

} Cmd;

Cmd* cmd();

void parse(char *cmdLine, Cmd* cmd);

Queue* queue();
void storehistory(char *cmdLine,Queue* queue);

#include "parse.c"
#endif