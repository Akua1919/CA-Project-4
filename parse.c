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
    cmd->background = 0;
    cmd->writekind = 0;
    cmd->number = 0;

    for (int i = 0; i < PIPE; i++)
	{
		cmd->cmd[i].args = malloc(20*sizeof(char*));
		cmd->cmd[i].infd = 0;
		cmd->cmd[i].outfd = 1;
	}

    return cmd;
}

history* his()
{
    history* history_list;
    history_list = malloc(sizeof(history));

    history_list->minus =0;
    history_list->plus =0;

    for (int i = 0; i < 1024; i++)
    {
        history_list->history[i].number = i;
        history_list->history[i].state = "no";
        history_list->history[i].command = "";
    }
    return history_list;
}

void parse(char *Line, Cmd* cmd){

    char* element = strtok(Line," ");
    
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
                cmd->writekind = 1;
            }
            else
            {
                cmd->writekind = 2;
            }
            element = strtok(NULL," ");
            cmd->output = element;
            element = strtok(NULL," ");
        }
        else if (!strcmp(element,"|"))
        {
            element = strtok(NULL," ");
            int i = 0;
            while (strcmp(element,"|") && strcmp(element,">") && strcmp(element,"<") && strcmp(element,"&") && strcmp(element,">>"))
            {
                cmd->cmd[cmd->number].args[i] = element;
                element = strtok(NULL," ");
                i++;
                if (element == NULL)
                {
                    cmd->number++;
                    return;
                }
                
            }
            cmd->number++;
        }
        else if (!strcmp(element,"&"))
        {
            cmd->background = 1;
            element = strtok(NULL," ");
        }
        else
        {
            int i = 0;
            while (strcmp(element,"|") && strcmp(element,">") && strcmp(element,"<") && strcmp(element,"&") && strcmp(element,">>"))
            {
                cmd->cmd[cmd->number].args[i] = element;
                element = strtok(NULL," ");
                i++;
                if (element == NULL)
                {
                    cmd->number++;
                    return;
                }
                
            }
            cmd->number++;
            
        }
    }
}