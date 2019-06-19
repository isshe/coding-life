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

void str_cli(FILE *fp, int sockfd)
{
    int maxfd, stdineof = 0;
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for (; ;) {
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfd = max(fileno(fp), sockfd) + 1;
        isshe_select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if (isshe_readline(sockfd, recvline, MAXLINE) == 0) {
                if (stdineof == 1) {
                    return;     // 正常结束
                } else {
                    isshe_error_quit("str_cli: server terminated prematurely");
                }
            }

            isshe_fput(recvline, stdout);
        }

        if (FD_ISSET(fileno(fp), &rset)) {
            if (isshe_fgets(sendline, MAXLINE, fp) == NULL) {
                stdineof = 1;
                isshe_shutdown(sockfd, SHUT_WR);    // send FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            }

            isshe_write(sockfd, sendline, strlen(sendline));
        }
    }

}