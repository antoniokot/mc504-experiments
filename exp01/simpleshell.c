// Desevolvido por: Antônio Hideto Borges Kotsubo (236041) - MC504_A

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STDOUT 1
#define FAIL_CODE -1
#define FALSE 0
#define TRUE 1

int main(int argc, char **argv) {
  extern char **environ;
  char command[100];

  printf("simple-shell$: ");
  scanf("%s", command);

  char *command_token = strtok(command, " ");
  int  args_len = 0;
  while (command_token != NULL) {
    args_len++;
    printf("arg: %s\n", command_token);
    command_token = strtok(NULL, " ");
  }

  char *args[args_len+1];

  command_token = strtok(command, " ");
  int i = 0;
  while (command_token != NULL) {
    args[i] = command_token;
    i++;
    command_token = strtok(NULL, " ");
  }

  args[i] = NULL;

  char *path_token = strtok(argv[1], ":");
  int success = 0;
  while (path_token != NULL) {
    char path[strlen(path_token)];
    strcpy(path, path_token);
    strcat(path, "/");
    strcat(path, args[0]);

    printf("path: %s\n", path);

    pid_t pid = fork();
    if (pid == 0) {
      if (execve(path, args, environ) != FAIL_CODE) {
        success = 2;
      } 
    } else {
      waitpid(pid, 0, 0);
    }
    printf("path_token: %s\n", path_token);
    path_token = strtok(NULL, ":");
  }

  return 0;
}
