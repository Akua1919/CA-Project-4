#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"

/*
void printPrompt()
{
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    printf("\n%s", pwd->pw_name);

    char host[100];
    gethostname(host,sizeof(host));
    printf("@%s", host);

    char directory[256];
    getcwd(directory,sizeof(directory));
    printf(":%s$ ", directory);
}
*/

int excute_buildincommand(Cmd* command){
    if(strcmp(command->command_list[0],"exit")==0){
        exit(0);
    }
    /*
    else if(strcmp(command->command_list[0],"cd")==0){
        if(chdir(command->argument_list[0])){
            printf("bash: cd: %s: no such directory\n",command[1]);
        }
        return 1;
    }
    */
    else if(strcmp(command->command_list[0],"pwd")==0){
        char directory[256];
        getcwd(directory,sizeof(directory));
        printf("%s\n", directory);
        return 1;
    }
    return 0;
}


int main (int argc, char **argv)
{
    Queue* cmd_history;
    cmd_history = queue();
    /*
    Queue* job_history;
    job_history = queue();
    */
    FILE* input = fopen(argv[1],"r");

    while(!feof(input)){
        char cmdLine[1024];
        fgets(cmdLine,1024,input);
        
        int childPid;
        //printPrompt();
        
        for (int i = 0; i < 1024; i++)
        {
            if (cmdLine[i] == '\n')
            {
                cmdLine[i] = '\0';
                break;
            }
        }

        /*
        处理空行
        */
        
        Cmd* command;
        command = cmd();
        parse(cmdLine,command);

        storehistory(cmdLine,cmd_history);
        /*
        printf("input:%s\n",command->input);
        printf("output:%s\n",command->output);
        printf("%d\n",command->background);
        printf("%d\n",command->pipe);
        printf("command:%s\n",command->command_list[command->pipe]);
        printf("argument:%s\n",command->argument_list[command->pipe]);
        */
        if (excute_buildincommand(command))
        {
        }
        else
        {
            childPid = fork();
            if(childPid == 0){
                char** a = malloc(2*sizeof(char*));
                a[0] = command->command_list[0];
                a[1] = command->argument_list[0];
                if(execvp(a[0],a)<0){
                    printf("%s: command not found\n",command->command_list[0]);
                    exit(0);
                }
            }
            else
            {
            }
        }
        //printf("\n");
    }
    
}
