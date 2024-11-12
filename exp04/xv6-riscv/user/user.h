struct stat;
struct Node;

// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(const char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int lseek(int fd, int offset, int whence);
int throughput(void);
int fairness(void);
int moverhead(void);
int storemoverhead(int, int, int);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...) __attribute__ ((format (printf, 2, 3)));
void printf(const char*, ...) __attribute__ ((format (printf, 1, 2)));
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);

// umalloc.c
void* malloc(uint);
void free(void*);

// file system efficiency
#define MAX_CALLS 1000
struct file_efficiency_metrics {
  uint64 file_write_duration[MAX_CALLS];
  int file_write_count;
  uint64 file_read_duration[MAX_CALLS];
  int file_read_count;
  uint64 file_delete_duration[MAX_CALLS];
  int file_delete_count;
};

// random.c
uint random_range(int, int);
uint random(int);

// graph.c
void solve_shortest_paths(int);

// cpu_bound.c
void run_cpu_bound_experiment();

// io_bound.c
void run_io_bound_experiment(int[2]);

// random_write.c
void swap_chars(int, int, int, struct file_efficiency_metrics*);
void random_write(int[2], struct file_efficiency_metrics*);

// metrics.c
void get_metrics(int[2]);

void clear_duration_count(struct file_efficiency_metrics*);
void register_read_duration(uint64, struct file_efficiency_metrics*);
void register_write_duration(uint64, struct file_efficiency_metrics*);
void register_delete_duration(uint64, struct file_efficiency_metrics*);
void calculate_file_efficiency(int[2]);
