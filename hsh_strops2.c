#include "hsh.h"

/**
 * hsh_strcpy - Copy string 'src' to string 'dest'
 * @dest: Destination string pointer
 * @src: Source string pointer
 *
 * Return: point to 'dest'
 */
char *hsh_strcpy(char *dest, const char *src)
{
	size_t i = 0;

	while ((*(dest + i) = *(src + i)))
		i++;
	return (dest);
}

/**
 * hsh_strlen - return length of string 's'
 * @s: string to check
 *
 * Return: length of 's'
 */
size_t hsh_strlen(const char *s)
{
	size_t len;

	for (len = 0; *(s++) != '\0'; len++)
		;
	return (len);
}

/**
 * hsh_strcat - concatenate string `src' onto string `dest'
 * @dest: destination string
 * @src: source string
 *
 * Return: pointer to resulting string `dest'
 */
char *hsh_strcat(char *dest, const char *src)
{
	size_t i = 0, j = 0;

	while (dest[i])
		++i;
	while ((dest[i] = src[j]))
		++i, ++j;
	return (dest);
}

/**
 * hsh_strchr - locate character `c' in string `s'
 * @s: string to search
 * @c: character to search for
 *
 * Return: if `c' found, pointer to `s'; else, NULL
 */
char *hsh_strchr(const char *s, int c)
{
	size_t i = 0;

	while (s[i])
	{
		if (s[i] == c)
			return ((char *) s + i);
		++i;
	}
	if (s[i] == c)
		return ((char *) s + i);
	return (NULL);
}
