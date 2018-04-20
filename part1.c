#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

void printHelp()
{
    printf("exit - exit this application \n run X - execute application X, where X can be an application name, full path to application, or relative path to application.\n help - Print list Of Commands\n");
}

void handle()
{
    
}

int main()
{
  printf("Type 'help' for a list of commands \n");
  char command[255];
  int counter;
  sigset_t mask, prev;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGSTOP);
  while (1)
  {
    printf(" -> ");
    fgets(command, 255 ,stdin);
    int n = strlen(command);
    if (n > 0 && command[n - 1] == '\n')
      command[n-1] = '\0';
    if (strcmp(command, "exit") == 0)
    {
      exit(0);
    }
    else if (strcmp(command, "help") == 0)
    {
      printHelp();
    }
    else
    {
      char *token = strtok(command, " ");
      if(strcmp(token, "run") == 0)
      {
        char* args[5] = {NULL, NULL, NULL, NULL, NULL};
        counter = 0;
        token = strtok(NULL, " ");
        while(token != NULL)
        {
          args[counter] = token;
          counter++;
          token = strtok(NULL, " ");
        }
        args[counter] == NULL;
        pid_t pid;

        sigprocmask(SIG_BLOCK, &mask, &prev); 
        if ((pid = fork()) < 0) {     
            printf("Fork Error \n");
            exit(1);
        }
        else if(pid == 0)
        {
          sigprocmask(SIG_SETMASK, &prev, NULL); 
          if (execvp(args[0], args) < 0) 
          {     
            if (execv(args[0], args) < 0) 
            {     
                  printf("Exe failed\n");
                  exit(1);
            }
          }
        }
        else
        {
          wait(NULL);
          signal(SIGINT, handle);
          signal(SIGSTOP, handle);
          sigprocmask(SIG_SETMASK, &prev, NULL); 
          signal(SIGINT, SIG_DFL);
          signal(SIGSTOP, SIG_DFL);
        }
      }
      else
      {
        printf("Invalid Input \n");
      }
    }
  }
}

