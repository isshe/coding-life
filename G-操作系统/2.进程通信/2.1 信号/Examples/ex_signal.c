#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int sig)
{
    static int beeps = 0;

    if (++beeps < 5) {
        printf("BEEP\n");
        alarm(1);
    } else {
        printf("BOOM!\n");
        exit(0);        // exit process
    }
}

int main(void) {
    signal(SIGALRM, handler);
    alarm(1);
    while(1) {
        ;
    }
    exit(0);
}