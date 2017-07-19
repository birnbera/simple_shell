#include "hsh.h"

/**
 * get_next_line - wrapper to `getline()' to perform error checking
 * and exit if necessary.
 * @state: struct of current state of shell variables
 */
void get_next_line(struct hsh_state *state)
{
	errno = 0;
	if ((getline(&state->lineptr, &state->linesize, stdin)) == -1)
	{
		if (errno)
		{
			printerror(state, "getline");
		}
		exit_and_free(state);
	}
}

/**
 * tokenize - routine to tokenize string using `hsh_strtok()' and save
 * tokens in a ragged array of duplicated strings.
 * @state: struct of current state of shell variables
 */
void tokenize(struct hsh_state *state)
{
	char **command, **tmp, *line, *tok, *delims = "\n\t\r\v ";
	size_t argc, malloc_size = 16;

	line = state->lineptr;
	if (line == NULL)
	{
		printerror(state, "tokenize");
		exit_and_free(state);
	}
	command = malloc(sizeof(char *) * malloc_size);
	if (command == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	tok = hsh_strtok(line, delims);
	for (argc = 0; tok; argc++)
	{
		command[argc] = strdup(tok);
		if (command[argc] == NULL)
		{
			printerror(state, "strdup");
			exit_and_free(state);
		}
		tok = hsh_strtok(NULL, delims);
		if (argc == malloc_size - 1)
		{
			malloc_size *= 2;
			tmp = realloc(command, sizeof(char *) * malloc_size);
			if (tmp == NULL)
			{
				printerror(state, "realloc");
				exit_and_free(state);
			}
			command = tmp;
		}
	}
	command[argc] = tok;
	free_command(state);
	state->command = command;
}

/**
 * hsh_strtok - clone of `strtok()' using a static buffer
 * @str: string to tokenize
 * @delim: string of delimeters to test for
 *
 * Return: pointer to current token after replacing downstream delimeter with
 * null byte.
 */
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
