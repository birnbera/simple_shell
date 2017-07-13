#include "hsh.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

char *get_next_line(void)
{
	static char *lineptr = NULL;
	size_t n;

	if (getline(&lineptr, &n, stdin) == -1)
	{
		putchar('\n');
		free(lineptr);
		if (errno)
		{
			perror("getline");
			return (NULL);
		}
		else
			exit_and_free(0);
	}
	return (lineptr);
}
