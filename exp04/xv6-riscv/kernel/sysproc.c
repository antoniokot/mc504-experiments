#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_lseek(void)
{
  int fd;
  int offset;
  int whence;
  struct file *f;

  argint(0, &fd);
  argint(1, &offset);
  argint(2, &whence);
  
  if((f = myproc()->ofile[fd]) == 0)
    return -1;

  if(f->type != FD_INODE)
    return -1;

  switch (whence) {
    case 0: // SEEK_SET
      if (offset < 0)
        return -1;
      f->off = offset;
      break;
    case 1: // SEEK_CUR
      if (f->off + offset < 0)
        return -1;
      f->off += offset;
      break;
    case 2: // SEEK_END
      if (f->ip->size + offset < 0)
        return -1;
      f->off = f->ip->size + offset;
      break;
    default:
      return -1;
  }

  return f->off;
}

void print_padded_int(uint64 num, int width) {
  char buffer[20];
  int length = 0;
  uint64 temp = num;
  do {
    temp /= 10;
    length++;
  } while (temp != 0);
  int padding = width - length;
  if (padding > 0) {
    for (int i = 0; i < padding; i++) {
      buffer[i] = '0';
    }
  }
  int index = padding > 0 ? padding : 0;
  temp = num;
  do {
    buffer[index + length - 1] = '0' + (temp % 10);
    temp /= 10;
    length--;
  } while (length > 0);

  buffer[width] = '\0';
  printf("%s", buffer);
}

#define SCALE 1000000000
uint64
sys_get_io_latency_metrics(void)
{
  uint64 total_duration = 0;
  uint64 max_duration = 0;
  uint64 min_duration = io_latency_metrics.durations[0];
  for (int i = 0; i < io_latency_metrics.count; i++) {
    uint64 duration = io_latency_metrics.durations[i];
    if (duration > max_duration) max_duration = duration;
    if (duration < min_duration) min_duration = duration;
    total_duration += duration;
  }

  uint64 avg_io_latency = (total_duration * SCALE) / io_latency_metrics.count;
  uint64 normalized_io_latency = max_duration == min_duration ? SCALE : SCALE - ((avg_io_latency - (min_duration * SCALE)) * SCALE) / ((max_duration - min_duration) * SCALE);

  printf("Average IO Latency: %ld cycles\n", avg_io_latency / SCALE);
  printf("Normalized IO Latency: %ld.", normalized_io_latency / SCALE);
  print_padded_int(normalized_io_latency % SCALE, 9);
  printf("\n\n");

  io_latency_metrics.count = 0;
  memset(io_latency_metrics.durations, 0, sizeof(io_latency_metrics.durations));

  return 0;
}

uint64
sys_start_round() {
  start_round();
  return 0;
}

uint64
sys_calculate_throughput() {
  calculate_throughput();
  return 0;
}