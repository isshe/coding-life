#include "isshe_sbuf.h"
#include "isshe_unistd.h"
#include "isshe_common.h"
#include "isshe_ipc.h"

/* 临时文件 */
static char *sbuf_mktemp(char *template)
{
    size_t len;
    char *temp;

    len = strlen(template) + 1;
    temp = isshe_malloc(len);
    memcpy(temp, template, len);

    return mktemp(temp);    // mktemp会修改temp，因此temp是要可修改的
}

/* 创建一个n个槽的空的、有界的、共享先进先出缓冲区 */
void isshe_sbuf_init(isshe_sbuf_t *sp, int n, size_t size)
{
    sp->buf = isshe_calloc(n, sizeof(void *));
    sp->n = n;                       /* Buffer holds max of n items */
    sp->size = size;
    sp->front = sp->rear = 0;        /* Empty buffer iff front == rear */

    sp->sem_mutex_name = sbuf_mktemp(SEM_MUTEX_TEMPLATE);
    sp->sem_nempty_name = sbuf_mktemp(SEM_NEMPTY_TEMPLATE);
    sp->sem_nstored_name = sbuf_mktemp(SEM_NSTORED_TEMPLATE);
    printf("sem_mutex_name = %s\n", sp->sem_mutex_name);
    printf("sem_nempty_name = %s\n", sp->sem_nempty_name);
    printf("sem_nstored_name = %s\n", sp->sem_nstored_name);

    sp->mutex = isshe_sem_open(sp->sem_mutex_name, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 1);
    sp->nempty = isshe_sem_open(sp->sem_nempty_name, O_CREAT | O_EXCL, ISSHE_FILE_MODE, n);
    sp->nstored = isshe_sem_open(sp->sem_nstored_name, O_CREAT | O_EXCL, ISSHE_FILE_MODE, 0);
}

/* 清除、销毁 */
void isshe_sbuf_destroy(isshe_sbuf_t *sp)
{
    void *item;

    isshe_sem_wait(sp->mutex);                     /* Lock the buffer */
    while(isshe_sem_trywait(sp->nstored) != ISSHE_FAILURE) {   /* Wait for available item */
        item = sp->buf[(++sp->front)%(sp->n)];      /* Remove the item */
        isshe_free(item);
        isshe_sem_post(sp->nempty);                /* Announce available slot */
    }

    isshe_free(sp->buf);

    isshe_sem_post(sp->mutex);                     /* Unlock the buffer */
    isshe_sem_close(sp->mutex);
    isshe_sem_close(sp->nempty);
    isshe_sem_close(sp->nstored);
    isshe_sem_unlink(sp->sem_mutex_name);
    isshe_sem_unlink(sp->sem_nempty_name);
    isshe_sem_unlink(sp->sem_nstored_name);
    isshe_free(sp->sem_mutex_name);
    isshe_free(sp->sem_nempty_name);
    isshe_free(sp->sem_nstored_name);
}

/* 插入一个元素到缓冲区中 */
void isshe_sbuf_insert(isshe_sbuf_t *sp, void *item)
{
    void *new_item = isshe_malloc(sp->size);

    memcpy(new_item, item, sp->size);

    isshe_sem_wait(sp->nempty);                         /* Wait for available slot */
    isshe_sem_wait(sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = new_item;           /* Insert the item */
    isshe_sem_post(sp->mutex);                          /* Unlock the buffer */
    isshe_sem_post(sp->nstored);                        /* Announce available item */
}

/* 从缓冲区中删除一个元素 */
void isshe_sbuf_remove(isshe_sbuf_t *sp, void *output_item)
{
    void *item;
    isshe_sem_wait(sp->nstored);                   /* Wait for available item */
    isshe_sem_wait(sp->mutex);                     /* Lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];         /* Remove the item */
    isshe_sem_post(sp->mutex);                     /* Unlock the buffer */
    isshe_sem_post(sp->nempty);                    /* Announce available slot */
    if (output_item) {
        memcpy(output_item, item, sp->size);
    }

    if (item) {
        isshe_free(item);
    }
}