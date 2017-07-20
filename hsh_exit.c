#include "hsh.h"

/**
 * hsh_isdigit - checks whether input char is a digit
 * @c: char is check
 *
 * Return: 1 if true, 0 if false
 */
int hsh_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/**
 * isnum - utility to test whether a string is exclusively numeric
 * @str: string to check
 *
 * Return: 1 if it is numeric (decimal), 0 otherwise
 */
int isnum(char *str)
{
	while (str && *str)
	{
		if (!hsh_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

/**
 * exit_wrap - wrapper to exit_and_free for use as a builtin to
 * ensure that arguments to `exit' are of numeric type.
 * @state: struct containing shell state variables
 */
void exit_wrap(struct hsh_state *state)
{
	char errormsg[1024];

	if (isnum(state->command[1]))
	{
		if (state->command[1])
			state->status = hsh_atoi(state->command[1]);
		exit_and_free(state);
	}
	state->status = 2;
	hsh_strcpy(errormsg, "Illegal number: ");
	hsh_strcat(errormsg, state->command[1]);
	hsh_strcat(errormsg, "\n");
	printerror(state, errormsg);
}

/**
 * exit_and_free - function to free all mallocated memory and exit
 * gracefully following an error or call to `exit [...]'
 * @state: struct containing shell state variables
 */
void exit_and_free(struct hsh_state *state)
{
	free_command(state);
	free_env(state);
	free_lineptr(state);
	free_builtins(state);
	free_vars(state);
	free_aliases(state);
	if (errno == ENOENT)
		state->status = 127;
	exit(state->status);
}
