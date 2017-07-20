#include "hsh.h"

/**
 * conv_base - convert int to the base given
 * @s: array of digits
 * @base: the base to convert to
 * @n: int to be converted
 *
 * Return: length of the array
 */
int conv_base(char *s, size_t base, size_t n)
{
	size_t i;

	i = 0;
	do {
		if ((n % base) < 10)
			s[i] = (n % base) + '0';
		else
			s[i] = ((n % base) - 10);
		n /= base;
		++i;
	} while (n);
	return (i);
}

/**
 * print_prompt - simple function to print a prompt to stdout
 */
void print_prompt(void)
{
	hsh_puts("$ ");
}

/**
 * printerror - utility function to print the error message associated
 * with the currently set `errno'
 * @state: struct containing shell state variables
 * @msg: prefix to print before error string
 */
void printerror(struct hsh_state *state, const char * const msg)
{
	size_t writ, cmdcnt = state->commandcount;
	char strcmdcnt[64];

	writ = conv_base(strcmdcnt, 10, cmdcnt);
	hsh_puts(state->argv[0]);
	hsh_puts(": ");
	write(STDERR_FILENO, strcmdcnt, writ);
	hsh_puts(": ");
	if (!msg)
		perror(state->command[0]);
	else
	{
		hsh_puts(state->command[0]);
		hsh_puts(": ");
		hsh_puts(msg);
	}
}
