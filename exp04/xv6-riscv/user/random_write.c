#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define LINES 100
#define CHAR_PER_LINE 100
#define PERMUTATIONS 50
#define FILENAME "random_file.txt"

char random_char() {
    return '!' + random(94);
}

void swap_chars(int fd, int pos1, int pos2) {
    char buf1, buf2;

    lseek(fd, pos1, 0);
    read(fd, &buf1, 1);

    lseek(fd, pos2, 0);
    read(fd, &buf2, 1);

    lseek(fd, pos1, 0);
    write(fd, &buf2, 1);

    lseek(fd, pos2, 0);
    write(fd, &buf1, 1);
}

void random_write() {
    int fd = open(FILENAME, O_CREATE | O_WRONLY);
    if (fd == -1) {
        printf("Error opening file\n");
        exit(1);
    }

    char *buffer = malloc(LINES * (CHAR_PER_LINE + 1));
    if (buffer == 0) {
        printf("Error allocating memory\n");
        close(fd);
        exit(1);
    }

    char *buf_ptr = buffer;
    for (int i = 0; i < LINES; i++) {
        for (int j = 0; j < CHAR_PER_LINE; j++) {
            *buf_ptr++ = random_char();
        }
        *buf_ptr++ = '\n';
    }

    write(fd, buffer, LINES * (CHAR_PER_LINE + 1));

    free(buffer);

    fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        printf("Error reopening file\n");
        exit(1);
    }

    for (int i = 0; i < 50; i++) {
        int pos1 = random(LINES * (CHAR_PER_LINE + 1));
        int pos2 = random(LINES * (CHAR_PER_LINE + 1));
        if (pos1 != pos2) {
            swap_chars(fd, pos1, pos2);
        }
    }

    close(fd);
    if (unlink(FILENAME) == -1) {
        printf("Error deleting file\n");
        exit(1);
    }
}
