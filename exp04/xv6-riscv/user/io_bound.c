#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_io_bound_experiment(int rounds) {
    printf("Running IO-Bound experiment with %d rounds...\n\n", rounds);
    for(int i = 0; i < rounds; i++) {
        printf("Running round %d...\n", i + 1);
        random_write();
    }
}