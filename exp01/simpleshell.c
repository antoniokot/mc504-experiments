// Desevolvido por: Antônio Hideto Borges Kotsubo (236041) - MC504_A

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h> 

#define STDOUT 1
#define FAIL_CODE -1
#define FALSE 0

int main(int argc, char **argv) {
  char *path = argv[1];
  char *command[] = { "whoami", "--version", NULL };
  extern char **environ;

  pid_t pid = fork();
  if (pid == 0) {
    if (execve(path, command, environ) == FAIL_CODE) {
      printf("O comando não pode ser executado.");
    } 
  } else {
    waitpid(pid, 0, 0);
  }

  return 0;
}
