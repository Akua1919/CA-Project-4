#ifndef PARSE_H
#define PARSE_H
#define PIPE 10

typedef struct command
{
    char** args;
	int infd;
	int outfd;
} command;

typedef struct cmdline
{
    command cmd[PIPE];
    char* input;
    char* output;
    int background;
    int writekind;
    int number;

} Cmd;

typedef struct job
{
    int number;
    char* state;
    char* command;
} job_item;

typedef struct list
{
    job_item history[1024];
    int plus;
    int minus;
    int most;
} history;


history* his();
Cmd* cmd();
void parse(char* Line, Cmd* cmdline);

#include "parse.c"
#endif