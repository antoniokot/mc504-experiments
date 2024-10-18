// Definir as constantes do gerador congruente linear
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

unsigned int seed = 123456; // Semente inicial (pode ser alterada)

// Função que gera um número pseudoaleatório entre 0 e max (exclusivo)
unsigned int random(int max) {
    seed = (A * seed + C) % M;
    return seed % max;
}

// Função para alterar a semente inicial
void set_seed(unsigned int new_seed) {
    seed = new_seed;
}