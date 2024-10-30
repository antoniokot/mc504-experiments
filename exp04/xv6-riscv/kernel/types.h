typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

#define MAX_CALLS 1000000
struct io_latency_metrics {
    uint64 durations[MAX_CALLS];
    int count;
};

extern struct io_latency_metrics io_latency_metrics;
