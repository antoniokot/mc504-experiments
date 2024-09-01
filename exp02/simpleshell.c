/*
  Sistemas Operacionais - MC504A_2024S2
  Desenvolvido por: Antônio Hideto Borges Kotsubo (236041)
*/ 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define STDOUT 1
#define FAIL_CODE -1
#define FALSE 0
#define TRUE 1
#define MAX 100

int getNumberOfArguments(char *command) {
  char command_copy[MAX];
  strcpy(command_copy, command);
  int args_len = 0;

  char *command_token = strtok(command_copy, " ");             // split the command string by empty spaces
  while (command_token != NULL) {
    args_len++;                                                // get the number of arguments
    command_token = strtok(NULL, " ");
  }

  return args_len;
}

void buildArgumentsArray(char **args, char *command) {
  int i = 0;
  char *command_token = strtok(command, " ");                 // split the command string by empty spaces again
  while (command_token != NULL) {       
    args[i] = command_token;                                  // get all of the arguments
    i++;
    command_token = strtok(NULL, " ");
  }

  args[i] = NULL;
}

int shouldRunBackground(char *command) {
  int boolean = 0;
  size_t len = strlen(command);
  if(command[len - 1] == '&') {                               // checks to see if the command should run in background
    boolean = 1;
    command[len - 1] = '\0';
  }

  return boolean;
}

int main(int argc, char **argv) {
  char command[MAX];
  
  while(TRUE) {
    printf("simple-shell$: ");                             
    if (fgets(command, sizeof(command), stdin) == NULL) {     // get the command from the terminal
      continue;                                               // if fgets fails, continue to the next iteration
    }

    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {                // remove the new line character from the command
      command[len - 1] = '\0';
    }

    if (strcmp(command, "") == 0) {                           // checks to see if the command is empty
      continue;
    }

    if (strcmp(command, "exit") == 0) {                       // checks to see if the command is "exit"
      return 0;                                               // if it is, then terminates the shell
    }

    int background = shouldRunBackground(command);

    int args_len = getNumberOfArguments(command);             // get the number of arguments

    char *args[args_len + 1];
    buildArgumentsArray(args, command);                       // then build the arguments array

    char paths_copy[MAX];
    strcpy(paths_copy, argv[1]);                              // copies the paths passed when the program was started
    char *path_token = strtok(paths_copy, ":");               // separate the possible multiple paths

    pid_t pid = fork();                                       // forks the process

    if(pid == 0)  {
      if (background) {                                       // checks to see if the command should run in background
        int new_terminal = open("/dev/null", O_RDWR);         // we will redirect the son's output to another terminal
        if (new_terminal == -1) {
          perror("Não foi possível abrir outro terminal");
          exit(FAIL_CODE);
        }
        
        dup2(new_terminal, STDOUT_FILENO);                    // redirect stdout and stdin to the new terminal  
        dup2(new_terminal, STDIN_FILENO);
        close(new_terminal); 
      }

      while (path_token != NULL) {
        char path[strlen(path_token) + strlen(args[0]) + 2];  // create the path string
        strcpy(path, path_token);                             // build the path
        strcat(path, "/");
        strcat(path, args[0]);

        execv(path, args);                                    // executes the command passed by the user
        path_token = strtok(NULL, ":");
      }

      printf("Comando não encontrado.\n");                    // if the son process wasn't ended, then something went wrong
      exit(FAIL_CODE);
    } else if(!background) {
      waitpid(pid, NULL, 0);                                  // if the command shouldn't run in background, then the father process should wait
    }
  }

  return 0;
}
