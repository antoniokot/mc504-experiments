#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "user/fileeff.h"

#define LINES 100
#define CHAR_PER_LINE 100
#define PERMUTATIONS 50
#define SCALE 1000000000
#define FILENAME "random_file.txt"

struct file_efficiency_metrics file_efficiency_metrics =  { .file_write_count = 0, .file_read_count = 0, .file_delete_count = 0};

char random_char() {
    return '!' + random(94);
}

void swap_chars(int fd, int pos1, int pos2, int process_num) {
    char buf1, buf2;
    uint64 start_read, start_write, end_read, end_write;

    start_read = uptime();
    lseek(fd, pos1, 0);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_read = uptime();
    read(fd, &buf1, 1);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_read = uptime();
    lseek(fd, pos2, 0);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_read = uptime();
    read(fd, &buf2, 1);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_read = uptime();
    lseek(fd, pos1, 0);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_write = uptime();
    write(fd, &buf2, 1);
    end_write = uptime();
    file_efficiency_metrics.file_write_duration[process_num][file_efficiency_metrics.file_write_count++] = end_write - start_write;

    start_read = uptime();
    lseek(fd, pos2, 0);
    end_read = uptime();
    file_efficiency_metrics.file_read_duration[process_num][file_efficiency_metrics.file_read_count++] = end_read - start_read;

    start_write = uptime();
    write(fd, &buf1, 1);
    end_write = uptime();
    file_efficiency_metrics.file_write_duration[process_num][file_efficiency_metrics.file_write_count++] = end_write - start_write;
}

void random_write(int process_num) {
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

    uint64 start_write = uptime();
    write(fd, buffer, LINES * (CHAR_PER_LINE + 1));
    uint64 end_write = uptime();

    file_efficiency_metrics.file_write_duration[process_num][file_efficiency_metrics.file_read_count++] = end_write - start_write;

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
            swap_chars(fd, pos1, pos2, process_num);
        }
    }

    close(fd);
    uint64 start_delete = uptime();
    if (unlink(FILENAME) == -1) {
        printf("Error deleting file\n");
        exit(1);
    }
    uint64 end_delete = uptime();

    file_efficiency_metrics.file_delete_duration[process_num][file_efficiency_metrics.file_delete_count++] = end_delete - start_delete;
}
