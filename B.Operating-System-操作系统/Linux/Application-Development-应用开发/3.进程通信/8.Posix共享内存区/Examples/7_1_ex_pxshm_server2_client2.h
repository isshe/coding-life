#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_ipc.h"
#include "isshe_process.h"
#include "isshe_unistd.h"
#include "isshe_file.h"

#define MESGSIZE    256
#define NMESG       16

#define MUTEX_NAME      "mutex"
#define NEMPTY_NAME     "nempty"
#define NSTORED_NAME    "nstored"
#define NFMUTEX_NAME    "noverflowmutex"

struct shmstruct {
    //sem_t   mutex;
    //sem_t   nempty;
    //sem_t   nstored;
    int     nput;
    long    noverflow;        // 为了控制：有新的溢出，才进行显示
    //sem_t   noverflowmutex;
    long    msgoff[NMESG];              // 信息偏移
    char    msgdata[NMESG * MESGSIZE];  // 信息数据
};