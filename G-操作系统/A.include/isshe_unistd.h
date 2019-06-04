#ifndef _ISSHE_UNISTD_H_
#define _ISSHE_UNISTD_H_

#include <sys/mman.h>

long isshe_pathconf(const char *pathname, int name);
long isshe_sysconf(int name);
int isshe_fcntl(int fd, int cmd, void *arg);
int isshe_getopt(int argc, char *const *argv, const char *str);
void *isshe_calloc(size_t n, size_t size);
void *isshe_malloc(size_t size);

void *isshe_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void isshe_munmap(void *addr, size_t len);
void isshe_sleep_us(unsigned int nusecs);

#endif