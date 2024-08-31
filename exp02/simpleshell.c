// Desenvolvido por: Antônio Hideto Borges Kotsubo (236041) - MC504A_2024S2

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STDOUT 1
#define FAIL_CODE -1
#define FALSE 0
#define TRUE 1
#define MAX 100

int getNumberOfArguments(char *command) {
  char command_copy[MAX];
  strcpy(command_copy, command);
  int args_len = 0;
  
  char *command_token = strtok(command_copy, " ");          // split the command string by empty spaces
  while (command_token != NULL) {
    args_len++;                                             // get the number of arguments
    command_token = strtok(NULL, " ");
  }

  return args_len;
}

void getArguments(char **args, char *command) {
  int i = 0;
  char *command_token = strtok(command, " ");               // split the command string by empty spaces again
  while (command_token != NULL) {       
    args[i] = command_token;                                // get all of the arguments
    i++;
    command_token = strtok(NULL, " ");
  }

  args[i] = NULL;
}

int main(int argc, char **argv) {
  char command[MAX];
  
  while(TRUE) {
    printf("simple-shell$: ");                                // get the command from the terminal
    // get the command from the terminal
    if (fgets(command, sizeof(command), stdin) == NULL) {
      continue;                                               // if fgets fails, continue to the next iteration
    }

    size_t len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
      command[len - 1] = '\0';
    }

    if (strcmp(command, "") == 0) {
      continue;
    }

    if (strcmp(command, "exit") == 0) {
      break;
    }

    int args_len = getNumberOfArguments(command);

    char *args[args_len + 1];
    getArguments(args, command);

    char paths_copy[MAX];
    strcpy(paths_copy, argv[1]);
    char *path_token = strtok(paths_copy, ":");               // separate the possible multiple pahts
    int success = FALSE;

    while (path_token != NULL) {
      char path[strlen(path_token) + strlen(args[0]) + 2];    // create the path string
      strcpy(path, path_token);                               // build the path
      strcat(path, "/");
      strcat(path, args[0]);

      pid_t pid = fork();
      if (pid == 0) {                                         // child process
        execv(path, args);                                    // execute execv
        exit(FAIL_CODE);
      } else {                                                // not the child process
        int status;
        wait(&status);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {  // status of success
          success = TRUE;
          break;
        }
      }

      path_token = strtok(NULL, ":");
    }

    if (!success) {                                           // if the command has failed
      printf("Comando não encontrado.\n");
    }
  }

  return 0;
}
