#include "hsh.h"

env_t *copyenv(char *envp[])
{
	env_t *new, *prev, *envhead = NULL;
	char *name, *value;

	if (envp && *envp)
	{
		name = hsh_strtok(*envp, "=");
                value = hsh_strtok(NULL, NULL);
                new = create_venv((const char *) name, (const char *) value);
		if (new == NULL)
		{
			printerror("malloc");
			return (NULL);
		}
                new->next = NULL;
		envhead = prev = new;
                envp++;
	}
	while (envp && *envp)
	{
		name = hsh_strtok(*envp, "=");
		value = hsh_strtok(NULL, NULL);
		new = create_venv((const char *) name, (const char *) value);
		if (new == NULL)
		{
			printerror("malloc");
			break;
		}
		new->next = NULL;
		prev->next = new;
		envp++;
	}
	return (envhead);
}

char **env_to_arr(struct st_vars *vars)
{
	size_t i, combined_len, size = vars->envsize + 1;
	env_t *envhead = vars->env;
	char **envp;

	envp = malloc(sizeof(char *) * size);
	if (envp == NULL)
	{
		printerror("malloc");
		return (NULL);
	}
	for (i = 0; envhead; i++)
	{
		combined_len = strlen(envhead->name) + strlen(envhead->value);
		combined_len += 2; /* space for `=' and `\0' */
		envp[i] = malloc(sizeof(char) * combined_len);
		if (envp[i] == NULL)
		{
			printerror("malloc");
			break;
		}
		strcpy(envp[i], envhead->name);
		strcat(envp[i], "=");
		strcat(envp[i], envhead->value);
		envhead = envhead->next;
	}
	envp[i] = NULL;
	return (envp);
}

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

void hsh_env(struct st_vars *vars)
{
	env_t *envhead = vars->env;

	while (envhead)
	{
		hsh_puts(envhead->name);
		hsh_putchar('=');
		hsh_puts(envhead->value);
		hsh_putchar('\n');
		envhead = envhead->next;
	}
}

int hsh_unsetenv(struct st_vars *vars, const char *name)
{
	env_t *last, *envhead = vars->env;

	if (name == NULL || *name == '\0' || strchr(name, '='))
	{
		errno = EINVAL;
		printerror("unsetenv");
		return (-1);
	}
	if (envhead == NULL)
		return (0);
	if (strcmp(envhead->name, name) == 0)
	{
		vars->env = envhead->next;
		free(envhead->name);
		free(envhead->value);
		free(envhead);
		return (0);
	}
	last = envhead;
	envhead = envhead->next;
	while (envhead)
	{
		if (strcmp(envhead->name, name) == 0)
		{
			last->next = envhead->next;
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

int hsh_setenv(struct st_vars *vars,
		const char *name, const char *value, int overwrite)
{
	env_t *new, *envhead = var->env;
	char *valuecpy;

	if (name == NULL || *name == '\0' || strchr(name, '='))
	{
		errno = EINVAL;
		printerror("setenv");
		return (-1);
	}
	while (envhead && envhead->next)
	{
		if (overwrite && strcmp(envhead->name, name) == 0)
		{

			valuecpy = strdup(value);
			if (valuecpy == NULL)
			{
				errno = ENOMEM;
				printerror("setenv");
				return (-1);
			}
			free(envhead->value);
			envhead->value = valuecpy;
			return (0);
		}
		envhead = envhead->next;
	}
	new = create_venv(vars, name, value);
	if (new == NULL)
	{
		errno = ENOMEM;
		printerror("setenv");
		return (-1);
	}
	var->envsize++;
	if (envhead)
	{
		envhead->next = new;
		return (0);
	}
	var->env = new;
	return (0);
}

char *hsh_getenv(struct st_vars *vars, const char *name)
{
	env_t *envhead = vars->envh;

	while (envhead)
	{
		if (strcmp(envhead->name, name) == 0)
			return (envhead->value);
	}
	return (NULL);
}

env_t *create_venv(const char *name, const char *value)
{
	env_t *new = malloc(sizeof(env_t));

	if (new == NULL)
		return (NULL);;
	new->name = strdup(name);
	if (new->name = NULL)
	{
		free(new);
		return (NULL);
	}
	new->value = strdup(value);
	if (new->value = NULL)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}
