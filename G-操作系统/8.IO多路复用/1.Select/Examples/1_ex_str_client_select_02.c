#include "isshe_common.h"
#include "isshe_error.h"
#include "isshe_stdio.h"
#include "isshe_unistd.h"
#include "isshe_file.h"
#include "isshe_ipc.h"
#include "isshe_pthread.h"
#include "isshe_process.h"
#include "isshe_signal.h"
#include "isshe_socket.h"
#include "isshe_time.h"
#include "isshe_rio.h"
#include "isshe_rpc.h"
#include "isshe_sbuf.h"

// 这个实现解决了01里面的两个问题。
// read/write: 解决stdio缓冲区的问题。
// stdineof: 发送完成后，等待接收完成，这样就不会因为提前结束，而丢失数据。
void str_cli(FILE *fp, int sockfd)
{
    int maxfd, stdineof = 0;
    fd_set rset;
    char bug[MAXLINE];
    int n;

    FD_ZERO(&rset);
    for (; ;) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;
        isshe_select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if ( ( n = isshe_read(sockfd, buf, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return;     // 正常结束
                } else {
                    isshe_error_quit("str_cli: server terminated prematurely");
                }
            }

            // 输出到标准输出
            isshe_write(fileno(stdout), buf， n);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if ( (n = isshe_read(fileno(fp), buf, MAXLINE)) == NULL) {
                stdineof = 1;
                isshe_shutdown(sockfd, SHUT_WR);    // send FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            isshe_write(sockfd, buf, n);
        }
    }
}