#include <stdio.h>
#include "get_next_line.h"
#include <fcntl.h>
int main()
{
	char *l;

	int fd = open("text.txt", O_CREAT, O_RDONLY);
	l = get_next_line(fd);
	while (l)
	{
		printf("text : %s", l);
		free(l);
		l = get_next_line(fd);
	}

	return 0;
}
