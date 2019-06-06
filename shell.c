#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "parse.h"

int buildin(Cmd* cmdline, history* history_list)
{
    if (!strcmp(cmdline->cmd[0].args[0],"exit"))
    {
        exit(0);
    }
    else if (!strcmp(cmdline->cmd[0].args[0],"cd"))
    {
        int fd;
    	fd=open(cmdline->cmd[0].args[1],O_RDONLY);
    	fchdir(fd);
    	close(fd);
		return 1;
    }
    else if (!strcmp(cmdline->cmd[0].args[0],"jobs"))
	{
        for (int i = 0; i < history_list->most; i++)
		{
			if (strcmp(history_list->history[i].state,"no"))
			{
				if (i == history_list->plus-1)
				{
					printf("[%d]+  %s               %s\n",history_list->history[i].number,history_list->history[i].state,history_list->history[i].command);
				}
				else if (i == history_list->minus-1)
				{
					printf("[%d]-  %s               %s\n",history_list->history[i].number,history_list->history[i].state,history_list->history[i].command);
				}
				else
				{
					printf("[%d]   %s               %s\n",history_list->history[i].number,history_list->history[i].state,history_list->history[i].command);
				}	
			}
			
		}
		return 1;
	}
	else if (!strcmp(cmdline->cmd[0].args[0],"kill"))
	{
        /*
        */
		return 1;
	}
	else
	{
		return 0;
	}
    
    
}

int main (int argc, char **argv)
{
    FILE* input = fopen(argv[1],"r");
    history* history_list;
    history_list = his();

    while(!feof(input)){
        pid_t childPid;
        char Line[1024];
        fgets(Line,1024,input);
        
        for (int i = 0; i < 1024; i++)
        {
            if (Line[i] == '\n')
            {
                Line[i] = '\0';
                break;
            }
        }
        
        Cmd* cmdline;
        cmdline = cmd();
        parse(Line,cmdline);

        /*
        printf("input:%s\n",cmdline->input);
        printf("output:%s\n",cmdline->output);
        printf("%d\n",cmdline->background);
        printf("%d\n",cmdline->number);
        printf("%d\n",cmdline->writekind);
        printf("argument:%s\n",cmdline->cmd[0].args[0]);
        printf("argument:%s\n",cmdline->cmd[0].args[1]);
        */

        if (cmdline->number == 0)
		{
			continue;
		}
        
        if (buildin(cmdline,history_list))
        {
            continue;
        }

        if (strcmp(cmdline->input,""))
		{
			cmdline->cmd[0].infd = open(cmdline->input, O_RDONLY);
		}
 
		if (strcmp(cmdline->output,""))
		{
			if (cmdline->writekind == 2)
			{
				cmdline->cmd[cmdline->number-1].outfd = open(cmdline->output, O_WRONLY | O_CREAT| O_APPEND, 0777);
			}
				
			else
			{
				cmdline->cmd[cmdline->number-1].outfd = open(cmdline->output, O_WRONLY | O_CREAT| O_TRUNC, 0777);
			}
				
		}

        int i;
		int fd;
		int fds[2];
		for (i = 0; i < cmdline->number; ++i)
		{
			if (i < (cmdline->number-1))
			{
				pipe(fds);
				cmdline->cmd[i].outfd = fds[1];
				cmdline->cmd[i+1].infd = fds[0];
			}
            /////////////////////////////////////////////////////////////////
			childPid = fork();
	        if (childPid == 0)
	        {
                if (cmdline->cmd[i].infd == 0 && cmdline->background == 1)
			    {
                    cmdline->cmd[i].infd = open("/dev/null", O_RDONLY);
                }
		        if (cmdline->cmd[i].infd != 0)
		        {
			        close(0);
			        dup(cmdline->cmd[i].infd);
		        }
		        if (cmdline->cmd[i].outfd != 1)
		        {
			        close(1);
			        dup(cmdline->cmd[i].outfd);
		        }

		        int j;
		        for (j=3; j < 1024; ++j)
			    {
                    close(j);
                }

		        execvp(cmdline->cmd[i].args[0], cmdline->cmd[i].args);
	        }

	        else
            {
		        if (cmdline->background == 1)
                {
					memcpy(history_list->history[history_list->most].command,Line,strlen(Line)+1);
					history_list->history[history_list->most].number = history_list->most+1;
					history_list->history[history_list->most].state = "Runing";
					history_list->most++;
					history_list->plus = history_list->most;
					history_list->minus = history_list->most-1;
                }
		        else
		        {
			        wait(childPid);
		        }
	        }      
            ////////////////////////////////////////////////////////////////////
			if ((fd = cmdline->cmd[i].infd) != 0)
				close(fd);

			if ((fd = cmdline->cmd[i].outfd) != 1)
				close(fd);
		}
    }
    fclose(input);
}
