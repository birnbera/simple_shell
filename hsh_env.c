#include "hsh.h"

/**
 * copyenv - utility function to parse environment into linked list of
 * structs containing name and value of variables.
 * @state: struct containing shell state variables, such as environment
 * @envp: pointer to environment variable strings
 *
 * Return: pointer to head of linked list of environment structs
 */
env_t *copyenv(struct hsh_state *state, char *envp[])
{
	env_t *new, *prev, *envhead = NULL;
	char *name, *value;

	if (envp && *envp)
	{
		name = hsh_strtok(*envp, "=");
		value = hsh_strtok(NULL, "");
		new = create_venv((const char *) name, (const char *) value);
		if (new == NULL)
		{
			printerror(state, "malloc");
			return (NULL);
		}
		new->next = NULL;
		envhead = prev = new;
		envp++;
	}
	while (envp && *envp)
	{
		name = hsh_strtok(*envp, "=");
		value = hsh_strtok(NULL, "");
		new = create_venv((const char *) name, (const char *) value);
		if (new == NULL)
		{
			printerror(state, "malloc");
			break;
		}
		new->next = NULL;
		prev->next = new;
		prev = new;
		envp++;
	}
	return (envhead);
}

/**
 * env_to_arr - utility function to convert environment linked list
 * back to array of pointers to strings for use with `execve'
 * @state: struct of current shell state
 *
 * Return: pointer to beginning of array of strings representing environment
 */
char **env_to_arr(struct hsh_state *state)
{
	size_t i, combined_len, size = state->envsize + 1;
	env_t *envhead = state->env;
	char **envp;

	envp = malloc(sizeof(char *) * size);
	if (envp == NULL)
	{
		printerror(state, "malloc");
		return (NULL);
	}
	for (i = 0; envhead; i++)
	{
		combined_len = hsh_strlen(envhead->name) + hsh_strlen(envhead->value);
		combined_len += 2; /* space for `=' and `\0' */
		envp[i] = malloc(sizeof(char) * combined_len);
		if (envp[i] == NULL)
		{
			printerror(state, "malloc");
			break;
		}
		hsh_strcpy(envp[i], envhead->name);
		hsh_strcat(envp[i], "=");
		hsh_strcat(envp[i], envhead->value);
		envhead = envhead->next;
	}
	envp[i] = NULL;
	return (envp);
}

/**
 * countenv - utility function to count the number of elements in environment
 * linked list.
 * @env: pointer to head of environment linked list
 *
 * Return: Unsigned integer of number of total elements
 */
size_t countenv(env_t *env)
{
	size_t count = 0;

	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

/**
 * create_venv - utility function to add new node to environment linked list
 * @name: name of variable to add
 * @value: value of variable to add
 *
 * Return: pointer to struct containing node; NULL on failure
 */
env_t *create_venv(const char *name, const char *value)
{
	env_t *new = malloc(sizeof(env_t));

	if (new == NULL)
		return (NULL);
	new->name = hsh_strdup(name);
	if (new->name == NULL)
	{
		free(new);
		return (NULL);
	}
	new->value = hsh_strdup(value);
	if (new->value == NULL)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}
