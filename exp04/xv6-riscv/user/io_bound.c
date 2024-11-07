#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "fileeff.h"

void run_io_bound_experiment(int num_process) {
    printf("\nRunning IO-Bound processes...\n");

    for(int i = 0; i < num_process; i++) {
        int pid = fork();
        if (pid < 0) {
            printf("Fork failed");
            exit(1);
        } else if (pid == 0) {
            random_write(i);
            
            //int end_time = uptime();
            //write(pipe_fd[1], &end_time, sizeof(end_time));
            exit(0);
        } else {
            int retPid = wait(&pid);
            kill(retPid);
        }
    }
    printf("Finished IO-Bound processes\n");
}