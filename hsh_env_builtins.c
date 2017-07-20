#include "hsh.h"

/**
 * hsh_env - print list of environment variables to stdout
 * @state: struct of current state variables
 */
void hsh_env(struct hsh_state *state)
{
	env_t *envhead = state->env;

	while (envhead)
	{
		hsh_puts(envhead->name);
		hsh_putchar('=');
		hsh_puts(envhead->value);
		hsh_putchar('\n');
		envhead = envhead->next;
	}
}

/**
 * hsh_unsetenv - remove variable `name' if it is currently set in
 * the environment.
 * @state: struct of current state variables
 * @name: name of variable to remove
 *
 * Return: 0 on success, -1 on failure
 */
int hsh_unsetenv(struct hsh_state *state, const char *name)
{
	env_t *last, *envhead = state->env;

	if (name == NULL || *name == '\0' || hsh_strchr(name, '='))
	{
		errno = EINVAL;
		printerror(state, "unsetenv");
		return (-1);
	}
	if (envhead == NULL)
		return (0);
	if (hsh_strcmp(envhead->name, name) == 0)
	{
		state->env = envhead->next;
		state->envsize--;
		free(envhead->name);
		free(envhead->value);
		free(envhead);
		return (0);
	}
	last = envhead;
	envhead = envhead->next;
	while (envhead)
	{
		if (hsh_strcmp(envhead->name, name) == 0)
		{
			last->next = envhead->next;
			state->envsize--;
			free(envhead->name);
			free(envhead->value);
			free(envhead);
			return (0);
		}
		last = envhead;
		envhead = envhead->next;
	}
	return (0);
}

/**
 * hsh_setenv - add variable `name' with `value' to current environment. If
 * already present and `overwrite' is non-zero, replace current value.
 * @state: struct of current state variables
 * @name: name of variable to add
 * @value: value to initialize to
 * @overwrite: overwrite existing variable if non-zero
 *
 * Return: 0 on succes, -1 on failure
 */
int hsh_setenv(struct hsh_state *state,
		const char *name, const char *value, int overwrite)
{
	env_t *new, *envhead = state->env;
	char *valuecpy;

	if (name == NULL || *name == '\0' || hsh_strchr(name, '='))
	{
		errno = EINVAL;
		printerror(state, "setenv");
		return (-1);
	}
	while (envhead && envhead->next)
	{
		if (overwrite && hsh_strcmp(envhead->name, name) == 0)
		{

			valuecpy = hsh_strdup(value);
			if (valuecpy == NULL)
			{
				printerror(state, "setenv");
				exit_and_free(state);
			}
			free(envhead->value);
			envhead->value = valuecpy;
			return (0);
		}
		envhead = envhead->next;
	}
	new = create_venv(name, value);
	if (new == NULL)
	{
		printerror(state, "setenv");
		exit_and_free(state);
	}
	state->envsize++;
	if (envhead)
	{
		envhead->next = new;
		return (0);
	}
	state->env = new;
	return (0);
}

/**
 * hsh_getenv - retrieve value of variable `name' from environment
 * @state: struct of current state variables
 * @name: name of variable to retrieve
 *
 * Return: pointer to value of variable `name'
 */
char *hsh_getenv(struct hsh_state *state, const char *name)
{
	env_t *envhead = state->env;

	while (envhead)
	{
		if (hsh_strcmp(envhead->name, name) == 0)
			return (envhead->value);
		envhead = envhead->next;
	}
	return (NULL);
}
