#include "user/user.h"
#include <stdlib.h>
#include <time.h>

#define LINES 100
#define CHAR_PER_LINE 100
#define PERMUTATIONS 50

char random_char() {
    return '!' + rand() % 94;
}

void permute_lines(FILE *file) {
    int line1 = rand() % LINES;
    int line2 = rand() % LINES;
    char buffer1[CHAR_PER_LINE + 1];
    char buffer2[CHAR_PER_LINE + 1];

    fseek(file, line1 * (CHAR_PER_LINE + 1), SEEK_SET);
    fgets(buffer1, sizeof(buffer1), file);
    fseek(file, line2 * (CHAR_PER_LINE + 1), SEEK_SET);
    fgets(buffer2, sizeof(buffer2), file);

    fseek(file, line1 * (CHAR_PER_LINE + 1), SEEK_SET);
    fputs(buffer2, file);
    fseek(file, line2 * (CHAR_PER_LINE + 1), SEEK_SET);
    fputs(buffer1, file);
}

void random_write() {
    FILE *file;
    char filename[] = "random_chars.txt";
    char line[CHAR_PER_LINE + 1];

    srand(time(NULL));

    file = fopen(filename, "w+");
    if (file == NULL) {
        perror("Failed to create file");
        return 1;
    }

    for (int i = 0; i < LINES; i++) {
        for (int j = 0; j < CHAR_PER_LINE; j++) {
            line[j] = random_char();
        }
        line[CHAR_PER_LINE] = '\n';
        fwrite(line, sizeof(char), CHAR_PER_LINE + 1, file);
    }

    for (int i = 0; i < PERMUTATIONS; i++) {
        permute_lines(file);
    }

    fclose(file);

    if (remove(filename) != 0) {
        perror("Failed to delete file");
        return 1;
    }

    printf("File created, permuted, and deleted successfully.\n");
}