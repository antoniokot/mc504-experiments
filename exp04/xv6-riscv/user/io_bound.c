#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_io_bound_experiment(int num_process) {
    printf("\nRunning IO-Bound processes...\n");

    for(int i = 0; i < num_process; i++) {
        printf("\nRunning process number %d...\n", i+1);

        int pid = fork();
        if (pid < 0) {
            printf("Fork failed");
            exit(1);
        } else if (pid == 0) {
            random_write();
            exit(0);
        } else {
            int retPid = wait(&pid);
            kill(retPid);
        }
    }
}