#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// Definir as constantes do gerador congruente linear
#define A 214013
#define C 2531011
#define M 0xFFFFFFFF // 2^32 - 1

unsigned int seed = 123456; // Semente inicial (pode ser alterada)

unsigned int calc_rand() {
    seed = (A * seed + C) & M;
    return seed >> 15;
}

unsigned int random_range(int lower, int upper) {
    int range = upper - lower + 1;
    unsigned int rand_val = calc_rand() % range;
    return rand_val + lower;
}

unsigned int random(int max) {
    seed = (A * seed + C) % M;
    return seed % max;
}
