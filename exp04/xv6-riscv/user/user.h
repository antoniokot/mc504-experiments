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
int incremoverhcount(void);
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

// random.c
uint random_range(int, int);
uint random(int);

// graph.c
void solve_shortest_paths(int);

// cpu_bound.c
void run_cpu_bound_experiment(int);

// io_bound.c
void run_io_bound_experiment(int);

// random_write.c
void swap_chars(int fd, int pos1, int pos2, int process_num);
void random_write(int process_num);

// metrics.c
void get_metrics(int);

#define MAX_ROUND_MEMORY_OVERHEAD 100 // Número máximo de medições de throughput por rodada

struct mem_overhead {
  int memory_access_time;   // Tempo total de acesso à memória
  int memory_alloc_time;    // Tempo total de alocação de memória
  int memory_free_time;     // Tempo total de desalocação de memória
};

// Array para armazenar o throughput temporário a cada segundo
extern struct mem_overhead mem_overhead_temp[MAX_ROUND_MEMORY_OVERHEAD];
extern int mem_overhead_count;
