// Desevolvido por: Antônio Hideto Borges Kotsubo (236041) - MC504_A

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STDOUT 1
#define FAIL_CODE -1
#define FALSE 0

int main(int argc, char **argv) {
  extern char **environ;
  char command[100];

  printf("simple-shell$: ");
  scanf("%s", command);

  char *args[] = { command, NULL };
  char *path = strtok(argv[1], ":");

  while (path != NULL) {
    
    strcat(path, "/");
    strcat(path, command);

    printf("path: %s\n", path);

    pid_t pid = fork();
    if (pid == 0) {
      if (execve(path, args, environ) == FAIL_CODE) {
        printf("O comando não pode ser executado: ");
        perror("execve");
      } 
    } else {
      waitpid(pid, 0, 0);
    }
    
    path = strtok(NULL, ":");
  }



  return 0;
}
