#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void run_io_bound_experiment(int rounds) {
    for(int i = 0; i < rounds; i++) {
        random_write();
    }
}