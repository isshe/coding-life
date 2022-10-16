#include <linux/bpf.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// for function declare
#include "bpf/bpf.h"  // /kernel-src/tools/lib/bpf/bpf.h

int main(int argc, char **argv)
{
    int my_map, result, key, value;
    key = 1, value = 1234;

    my_map = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(key),
                            sizeof(value), 256, 0);
    if (my_map < 0) {
        printf("failed to create map '%s'\n", strerror(errno));
        /* likely not run as root */
        return 1;
    }

    result = bpf_map_update_elem(my_map, &key, &value, BPF_ANY); 
    if (result == 0)
    {
    	printf("Map updated with new element BPF_ANY\n"); 
    }
	else
	{
		printf("Failed to update map with new value: %d (%s) BPF_ANY\n\n", result, strerror(errno));
	}

	value = 5678;
	
	// create an element with the same key
	result = bpf_map_update_elem(my_map, &key, &value, BPF_NOEXIST);
	if (result == 0)
	{
		printf("Map updated with new element BPF_NOEXIST \n"); 
	}
	else
	{
		printf("Failed to update map with new value: %d (%s) BPF_NOEXIST\n", result, strerror(errno));
	}

	// update an element that doesn’t exist yet
	key = 12, value = 5678;
	result = bpf_map_update_elem(my_map, &key, &value, BPF_EXIST); 

	if (result == 0)
	{
		printf("Map updated with new element BPF_EXIST\n"); 
	}
	else
	{
		printf("Failed to update map with new value: %d (%s) BPF_EXIST \n", result, strerror(errno));
	}

    return 0;
 }
