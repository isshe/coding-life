#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "bpf/bpf.h"

int main(int argc, char **argv)
{
    int fd, result, key, value;
    fd = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(key),
                                        sizeof(value), 256, 0);

    if (fd < 0) {
        printf("failed to create map '%s'\n", strerror(errno));
        /* likely not run as root */

        return 1;
    }

    key=1;
    result = bpf_map_delete_elem(fd, &key);

    if (result == 0)
    {
        printf("Element deleted from the map\n");
    }
    else
    {
        printf("Failed to delete element from the map: %d (%s)\n",
                                        result, strerror(errno));
    }

    return 0;
}
