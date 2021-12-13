
#include "isshe_common.h"
#include "isshe_unistd.h"

int main(int argc, char *argv[])
{
    printf("SEM_NSEMS_MAX = %ld, SEM_VALUE_MAX = %ld\n", isshe_sysconf(_SC_SEM_NSEMS_MAX), isshe_sysconf(_SC_SEM_VALUE_MAX));

    exit(0);
}