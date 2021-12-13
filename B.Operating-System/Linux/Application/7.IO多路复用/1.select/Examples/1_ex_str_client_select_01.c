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


/*
 * 这个函数存在的问题：
 *  1. fp 读完并发送给sockfd，但是sockfd可能没有读完就结束了。
 *  2. 由于stdio的缓冲区问题，fgets和readline(我们的缓冲区)，都只操作了一行，缓冲区中可能还有未消费的数据。
 */
void str_cli(FILE *fp, int sockfd)
{
    int maxfd, stdineof = 0;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for (; ;) {
        FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;
        isshe_select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if (isshe_readline(sockfd, recvline, MAXLINE) == 0) {
                isshe_error_quit("str_cli: server terminated prematurely");
            }

            // 输出到标准输出
            isshe_fput(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if (isshe_fgets(sendline, MAXLINE, fp) == NULL) {
                return;
            }

            isshe_write(sockfd, sendline, strlen(sendline));
        }
    }

}