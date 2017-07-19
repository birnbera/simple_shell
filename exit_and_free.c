#include "hsh.h"

int isnum(char *str)
{
	while (str && *str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void init_state(struct hsh_state *state, char **argv, char **envp)
{
	builtin_t *builtin;
	builtin_t builtins[] = {
		{"exit", exit_and_free},
		{NULL, NULL}
	};
	size_t i;

	state->argv = argv;
	state->env = copyenv(state, envp);
	state->lineptr = NULL;
	state->command = NULL;
	state->vars = NULL;
	state->aliases = NULL;
	state->linesize = 0;
	state->commandcount = 0;
	state->envsize = countenv(state->env);
	state->builtins = malloc(sizeof(builtin_t *) * sizeof(builtins));
	if (state->builtins == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	for (i = 0; builtins[i].name; i++)
	{
		builtin = malloc(sizeof(builtin_t));
		if (builtin == NULL)
		{
			printerror(state, "malloc");
			exit_and_free(state);
		}
		builtin->name = strdup(builtins[i].name);
		builtin->func = builtins[i].func;
		state->builtins[i] = builtin;
	}
	builtin = malloc(sizeof(builtin_t));
	if (builtin == NULL)
	{
		printerror(state, "malloc");
		exit_and_free(state);
	}
	builtin->name = builtins[i].name;
	builtin->func = builtins[i].func;
	state->builtins[i] = builtin;
}

void exit_wrap(struct hsh_state *state)
{
	if (isnum(state->command[1]))
	{
		state->status = atoi(state->command[1]);
		exit_and_free(state);
	}
	state->status = 2;
	printerror(state, "exit");
	exit_and_free(state);
}

void exit_and_free(struct hsh_state *state)
{
	free_command(state);
	free_env(state);
	free_lineptr(state);
	free_builtins(state);
	free_vars(state);
	free_aliases(state);
	exit(state->status);
}

void free_command(struct hsh_state *state)
{
	size_t i;

	if (state->command == NULL)
		return;
	for (i = 0; state->command[i]; i++)
		free(state->command[i]);
	free(state->command);
}

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

void free_lineptr(struct hsh_state *state)
{
	if (state->lineptr)
		free(state->lineptr);
}

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
	free(builtins);
}

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
