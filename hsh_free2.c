#include "hsh.h"

/**
 * free_vars - subroutin to free generic shell variables
 * @state: struct containing shell state variables
 */
void free_vars(struct hsh_state *state)
{
	var_t *tmp, *varhead = state->vars;

	while (varhead)
	{
		tmp = varhead;
		varhead = varhead->next;
		free(tmp->varname);
		free(tmp->varval);
		free(tmp);
	}
}

/**
 * free_aliases - subroutine to free any aliases set by `alias'
 * @state: struct containing shell state variables
 */
void free_aliases(struct hsh_state  *state)
{
	alias_t *tmp, *aliashead = state->aliases;

	while (aliashead)
	{
		tmp = aliashead;
		aliashead = aliashead->next;
		free(tmp->alname);
		free(tmp->alvalue);
		free(tmp);
	}
}

/**
 * hsh_realloc - reallocate memory at `ptr' to `new_size'
 * @ptr: ptr to existing memory or NULL
 * @old_size: number of bytes originally allocated to `ptr'
 * @new_size: number of bytes to request
 *
 * Return: pointer to new memory, or `ptr' if _realloc fails
 */
void *hsh_realloc(void *ptr, size_t old_size, size_t new_size)
{
	char *new_ptr, *old_ptr;
	size_t i;

	if (new_size == old_size)
		return (ptr);
	if (ptr != NULL && new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (ptr == NULL)
		return (new_ptr);
	old_ptr = ptr;
	for (i = 0; i < new_size && i < old_size; i++)
		new_ptr[i] = old_ptr[i];
	free(ptr);
	return (new_ptr);
}
