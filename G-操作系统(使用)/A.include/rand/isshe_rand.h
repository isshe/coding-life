
#ifndef _ISSHE_RAND_H_
#define _ISSHE_RAND_H_

//#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#define ISSHE_DEV_URANDOM "/dev/urandom"
//#endif

int isshe_rand_bytes(unsigned char *buf, int num);

#endif