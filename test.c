#include <stdio.h>
#include "get_next_line.h"
#include <fcntl.h>
#include <string.h>
int main()
{
	char *l;

	int fd = open("text.txt", O_RDONLY);
	l = get_next_line(fd);
	while (l)
	{
		fputs(l, stdout);
		free(l);
		l = get_next_line(fd);
	}
	close(fd);
	return 0;
}
