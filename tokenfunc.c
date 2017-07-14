#include <stddef.h>

char *hsh_strtok(char *str, const char *delim)
{
	static char *next;
	char *tok = NULL;
	size_t i;

	if (delim == NULL)
		return (str);
	if (str)
		next = str;
	while (next && *next)
	{
		for (i = 0; delim[i]; i++)
			if (*next == delim[i])
				break;
		if (delim[i] == '\0')
		{
			tok = next;
			break;
		}
		next++;
	}
	while (next && *next)
	{
		for (i = 0; delim[i]; i++)
		{
			if (*next == delim[i])
			{
				*next++ = '\0';
				return (tok);
			}
		}
		next++;
	}
	if (next && *next == '\0')
		next = NULL;
	return (tok);
}
