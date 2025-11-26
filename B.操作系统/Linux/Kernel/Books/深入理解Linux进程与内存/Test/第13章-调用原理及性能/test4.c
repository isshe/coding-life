#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char	c;
	int		in;
	int		i;

	in = open("data.out", O_RDONLY);
	for(i=0; i<1000000; i++){
		read(in,&c,1);
	}
	return 0;
}
