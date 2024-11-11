#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define LINES 100
#define CHAR_PER_LINE 100
#define PERMUTATIONS 50
#define FILENAME "random_file.txt"

int io_alloc_time = 0;
int io_free_time = 0;
int io_access_time = 0;

char random_char() {
    return '!' + random(94);
}

void swap_chars(int fd, int pos1, int pos2, struct file_efficiency_metrics* file_efficiency_metrics) {
    char buf1, buf2;
    uint64 start_read, start_write, end_read, end_write;

    start_read = uptime();
    lseek(fd, pos1, 0);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_read = uptime();
    read(fd, &buf1, 1);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_read = uptime();
    lseek(fd, pos2, 0);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_read = uptime();
    read(fd, &buf2, 1);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_read = uptime();
    lseek(fd, pos1, 0);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_write = uptime();
    write(fd, &buf2, 1);
    end_write = uptime();
    register_write_duration(end_write - start_write, file_efficiency_metrics);

    start_read = uptime();
    lseek(fd, pos2, 0);
    end_read = uptime();
    register_read_duration(end_read - start_read, file_efficiency_metrics);

    start_write = uptime();
    write(fd, &buf1, 1);
    end_write = uptime();
    register_write_duration(end_write - start_write, file_efficiency_metrics);
}

void random_write(int fs_pipe_fd[2], struct file_efficiency_metrics* file_efficiency_metrics) {
    int fd = open(FILENAME, O_CREATE | O_WRONLY);
    if (fd == -1) {
        printf("Error opening file\n");
        exit(1);
    }

    int start_alloc_time = uptime();
    char *buffer = malloc(LINES * (CHAR_PER_LINE + 1));
    int end_alloc_time = uptime();

    io_alloc_time += end_alloc_time - start_alloc_time;

    int start_access_time = uptime();
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

    uint64 start_write = uptime();
    write(fd, buffer, LINES * (CHAR_PER_LINE + 1));
    uint64 end_write = uptime();

    int end_access_time = uptime(); 
    io_access_time += end_access_time - start_access_time;

    register_write_duration(end_write - start_write, file_efficiency_metrics);

    int start_free_time = uptime();
    free(buffer);
    int end_free_time = uptime();
    io_free_time += end_free_time - start_free_time;

    fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        printf("Error reopening file\n");
        exit(1);
    }

    for (int i = 0; i < 50; i++) {
        int pos1 = random(LINES * (CHAR_PER_LINE + 1));
        int pos2 = random(LINES * (CHAR_PER_LINE + 1));
        if (pos1 != pos2) {
            swap_chars(fd, pos1, pos2, file_efficiency_metrics);
        }
    }

    close(fd);
    uint64 start_delete = uptime();
    if (unlink(FILENAME) == -1) {
        printf("Error deleting file\n");
        exit(1);
    }
    uint64 end_delete = uptime();

    register_delete_duration(end_delete - start_delete, file_efficiency_metrics);
    storemoverhead(io_alloc_time, io_free_time, io_access_time);
}
