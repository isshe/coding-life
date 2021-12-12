#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "bpf/bpf.h"

int main(int argc, char **argv)
{
    int fd, result, key, value, it;
    fd = bpf_create_map(BPF_MAP_TYPE_HASH, sizeof(key),
                                        sizeof(value), 256, 0);

    if (fd < 0) {
        printf("failed to create map '%s'\n", strerror(errno));
        return 1;
    }

    for( it = 1; it < 6; it++ ){
        key = it;
        value = it + 1234;

        bpf_map_update_elem(fd, &key, &value, BPF_NOEXIST);
    }

    int next_key, lookup_key;

    lookup_key = -1;
    while ( bpf_map_get_next_key( fd, &lookup_key, &next_key) == 0)
    {
        printf("The next key in the map is: '%d'\n", next_key);
        lookup_key = next_key;
    }

    lookup_key = -1;
    printf("\nDelete key test\n");
    while ( bpf_map_get_next_key( fd, &lookup_key, &next_key) == 0)
    {
        printf("The next key in the map is: '%d'\n", next_key);
        if (next_key == 2)
        {
            printf("Deleting key '2'\n");
            bpf_map_delete_elem(fd, &next_key);
        }

        lookup_key = next_key;
    }
    
    return 0;
}
