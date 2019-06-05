#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

Cmd* cmd()
{
    Cmd* cmd;
    cmd = malloc(sizeof(Cmd));
    cmd->input = "";
    cmd->output = "";
    cmd->pipe = 0;
    cmd->background = 0;
    cmd->opkind = 0;
    cmd->command_list = malloc(sizeof(char*));
    cmd->command_list[0] = "";
    cmd->argument_list = malloc(sizeof(char*));
    cmd->argument_list[0] = "";
    return cmd;
}

void parse(char *cmdLine2, Cmd* cmd){
    char* cmdLine = malloc(1024*sizeof(char));
    memcpy(cmdLine, cmdLine2, strlen(cmdLine2)+1);
    
    char* element = strtok(cmdLine," ");
    
    while (element != NULL)
    {
        if (!strcmp(element,"<"))
        {
            element = strtok(NULL," ");
            cmd->input = element;
            element = strtok(NULL," ");
        }
        else if (!strcmp(element,">") || !strcmp(element,">>"))
        {
            if (!strcmp(element,">"))
            {
                cmd->opkind = 1;
            }
            else
            {
                cmd->opkind = 2;
            }
            element = strtok(NULL," ");
            cmd->output = element;
            element = strtok(NULL," ");
        }
        else if (!strcmp(element,"|"))
        {
            cmd->pipe++;
            element = strtok(NULL," ");
            cmd->command_list = realloc(cmd->command_list,(cmd->pipe+1)*sizeof(char*));
            cmd->argument_list = realloc(cmd->argument_list,(cmd->pipe+1)*sizeof(char*));
            cmd->command_list[cmd->pipe] = element;
            element = strtok(NULL," ");
            if (element == NULL)
            {
                cmd->argument_list[cmd->pipe] = "";
            }
            else if (!strcmp(element,"<") || !strcmp(element,">") || !strcmp(element,">>") || !strcmp(element,"|") || !strcmp(element,"&"))
            {
                cmd->argument_list[cmd->pipe] = "";
            }
            else
            {
                cmd->argument_list[cmd->pipe] = element;
                element = strtok(NULL," ");
            }
        }
        else if (!strcmp(element,"&"))
        {
            cmd->background = 1;
            element = strtok(NULL," ");
        }
        else
        {
            cmd->command_list[cmd->pipe] = element;
            element = strtok(NULL," ");
            if (element == NULL)
            {
                cmd->argument_list[cmd->pipe] = "";
            }
            else if (!strcmp(element,"<") || !strcmp(element,">") || !strcmp(element,">>") || !strcmp(element,"|") || !strcmp(element,"&"))
            {
                cmd->argument_list[cmd->pipe] = "";
            }
            else
            {
                cmd->argument_list[cmd->pipe] = element;
                element = strtok(NULL," ");
            }
        }
    }
}

Queue* queue()
{
    Queue* queue;
    queue = malloc(sizeof(Queue));
    queue->loc = 0;
    queue->size = 1;
    queue->history = malloc(sizeof(char*));
    queue->history[0] = "";
    return queue;
}

void storehistory(char *cmdLine2,Queue* queue){
    char* cmdLine = malloc(1024*sizeof(char));
    memcpy(cmdLine, cmdLine2, strlen(cmdLine2)+1);

    if (queue->loc+1 <= queue->size)
    {
        queue->history[queue->loc] = cmdLine;
    }
    else
    {
        queue->history = realloc(queue->history,2*(queue->size)*sizeof(char*));
        queue->history[queue->loc] = cmdLine;
        queue->size = 2*queue->size;
    }
    queue->loc ++;
}