#include "hsh.h"

/**
 * free_command - subroutine to free command member variable of state
 * @state: struct containing shell state variables
 */
void free_command(struct hsh_state *state)
{
	size_t i;

	if (state->command == NULL)
		return;
	for (i = 0; state->command[i]; i++)
		free(state->command[i]);
	free(state->command);
}

/**
 * free_env - subroutine to free environment linked list
 * @state: struct containing shell state variables
 */
void free_env(struct hsh_state *state)
{
	env_t *tmp, *envhead = state->env;

	while (envhead)
	{
		tmp = envhead;
		envhead = envhead->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

/**
 * free_lineptr - subroutine to free lineptr member of `state'
 * @state: struct containing shell state variables
 */
void free_lineptr(struct hsh_state *state)
{
	if (state->lineptr)
		free(state->lineptr);
}

/**
 * free_builtins - subroutine to free list of builtin commands
 * @state: struct containing shell state variables
 */
void free_builtins(struct hsh_state *state)
{
	size_t i;
	builtin_t **builtins = state->builtins;

	if (!state->builtins)
		return;
	for (i = 0; builtins[i]->name; i++)
	{
		free(builtins[i]->name);
		free(builtins[i]);
	}
	free(builtins[i]); /* {NULL, NULL} */
	free(builtins);
}
