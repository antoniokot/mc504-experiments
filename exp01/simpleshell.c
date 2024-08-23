// Desenvolvido por: Antônio Hideto Borges Kotsubo (236041) - MC504_A

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

int main(int argc, char **argv) {
  char command[100];

  printf("simple-shell$: ");
  scanf(" %[^\n]", command);

  char command_copy[100];
  strcpy(command_copy, command);

  int args_len = 0;
  char *command_token = strtok(command_copy, " ");
  while (command_token != NULL) {
    args_len++;
    command_token = strtok(NULL, " ");
  }

  char *args[args_len + 1];

  int i = 0;
  command_token = strtok(command, " ");
  while (command_token != NULL) {
    args[i] = command_token;
    i++;
    command_token = strtok(NULL, " ");
  }

  args[i] = NULL;

  char *path_token = strtok(argv[1], ":");
  int success = FALSE;

  while (path_token != NULL) {
    char path[strlen(path_token) + strlen(args[0]) + 2];
    strcpy(path, path_token);
    strcat(path, "/");
    strcat(path, args[0]);

    pid_t pid = fork();
    if (pid == 0) {
      execv(path, args);
      exit(FAIL_CODE);
    } else {
      int status;
      wait(&status);

      if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        success = TRUE;
        break;
      }
    }

    path_token = strtok(NULL, ":");
  }

  if (!success) {
    printf("Program does not exist.\n");
    exit(1);
  }

  return 0;
}