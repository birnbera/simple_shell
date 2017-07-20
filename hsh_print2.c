#include "hsh.h"

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
