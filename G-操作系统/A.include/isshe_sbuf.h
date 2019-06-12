/*
 * 有界先进先出(FIFO)共享队列。
 * 注意：
 *  * 当阻塞在某个信号量时，需要先关闭阻塞线程/进程才能调用destory销毁信号量。
 *      * 因为sem_close()因为sem_wait()阻塞而阻塞。
 */

#ifndef _ISSHE_SBUF_H_
#define _ISSHE_SBUF_H_

#include <semaphore.h>

#define SEM_MUTEX_TEMPLATE   "mutex.XXXXXX"
#define SEM_NEMPTY_TEMPLATE  "nempty.XXXXXX"
#define SEM_NSTORED_TEMPLATE "nstored.XXXXXX"

typedef struct {
    void **buf;         /* Buffer array */
    int n;              /* Maximum number of slots */
    int size;           /* slots size */
    int front;          /* buf[(front+1)%n] is first item */
    int rear;           /* buf[rear%n] is last item */
    char *sem_mutex_name;
    char *sem_nempty_name;
    char *sem_nstored_name;
    sem_t *mutex;       /* Protects accesses to buf */
    sem_t *nempty;      /* Counts available slots */
    sem_t *nstored;     /* Counts available items */
} isshe_sbuf_t;

void isshe_sbuf_init(isshe_sbuf_t *sp, int n, size_t size);
void isshe_sbuf_destroy(isshe_sbuf_t *sp);
void isshe_sbuf_insert(isshe_sbuf_t *sp, void *item);
void isshe_sbuf_remove(isshe_sbuf_t *sp, void *output_item);

#endif /* _ISSHE_SBUF_H_ */