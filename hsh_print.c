#include "hsh.h"

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
	(void)(state);
	perror(msg);
}

/**
 * hsh_puts - utility to write a string to stdout
 * @str: string to write verbatim
 */
void hsh_puts(const char * const str)
{
	size_t i;

	for (i = 0; str[i]; i++)
		;
	write(1, str, i);
}

/**
 * hsh_putchar - utility to write a single char to stdou
 * @c: char to write
 */
void hsh_putchar(const char c)
{
	write(1, &c, 1);
}

