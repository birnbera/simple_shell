#include "hsh.h"

/**
 * hsh_strdup - copy `str' and return pointer
 * @str: string to copy
 *
 * Return: pointer to copy of `str', or NULL on failure
 */
char *hsh_strdup(const char *str)
{
	size_t i, j;
	char *dup;

	if (str == NULL)
		return (NULL);
	for (i = 0; str[i]; ++i)
		;
	dup = malloc(sizeof(char) * (i + 1));
	if (dup == NULL)
		return (NULL);
	for (j = 0; j <= i; ++j)
		dup[j] = str[j];
	return (dup);
}

/**
 * hsh_strcmp - Compares two null-terminated byte strings lexicographically
 * @s1: first string
 * @s2: second string
 *
 * Description: The sign of the result is the sign of the difference between
 * the values of the first pair of characters that differ in the strings
 * being compared.
 *
 * Return: Negative difference if `s1' is before `s2'; 0 if equal; positive
 * difference if `s2' is before `s1'
 */
int hsh_strcmp(const char *s1, const char *s2)
{
	int i = 0;

	while (s1[i] == s2[i] && s1[i] && s2[i])
		++i;
	return (s1[i] - s2[i]);
}

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
