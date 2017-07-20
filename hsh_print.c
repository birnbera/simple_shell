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

/**
 * hsh_puts - utility to write a string to stdout
 * @str: string to write verbatim
 */
void hsh_puts(const char * const str)
{
	size_t i;

	for (i = 0; str[i]; i++)
		;
	write(STDERR_FILENO, str, i);
}

/**
 * hsh_putchar - utility to write a single char to stdou
 * @c: char to write
 */
void hsh_putchar(const char c)
{
	write(STDERR_FILENO, &c, 1);
}

/**
 * hsh_atoi - Convert string 's' to integer
 * @s: String to convert to int, potentially with preceding characters
 *
 * Return: int containing string
 */
int hsh_atoi(char *s)
{
	char c;
	int i = 0, sign = 1, tmp;

	while ((c = *s) > '\0' && (c < '0' || c > '9'))
	{
		if (c == '-')
			sign *= -1;
		++s;
	}
	while (c >= '0' && c <= '9')
	{
		tmp = i;
		i *= 10;
		i -= (c - '0');
		if (i > tmp)
			return (tmp * -sign);
		c = *(++s);
	}
	return (i * -sign);
}
