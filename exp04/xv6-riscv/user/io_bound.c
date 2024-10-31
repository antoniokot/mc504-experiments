#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_io_bound_experiment(int num_process, int pipe_fd[2]) {
    printf("\nRunning IO-Bound processes...\n");

    for(int i = 0; i < num_process; i++) {
        printf("\nRunning process number %d...\n", i+1);

        int pid = fork();
        if (pid < 0) {
            printf("Fork failed");
            exit(1);
        } else if (pid == 0) {
            random_write();
            int end_time = uptime(); // ou rdtime(), dependendo do que preferir
            write(pipe_fd[1], &end_time, sizeof(end_time));
            exit(0);
        } else {
            int retPid = wait(&pid);
            kill(retPid);
        }
    }
    //get_io_latency_metrics();
}