#include "hsh.h"

/**
 * isnum - utility to test whether a string is exclusively numeric
 * @str: string to check
 *
 * Return: 1 if it is numeric (decimal), 0 otherwise
 */
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

/**
 * set_builtins - function to set the array of builtins to the
 * relevant member variable of `state'
 * @state: struct of shell state variables
 */
void set_builtins(struct hsh_state *state)
{
	builtin_t *builtin;
	builtin_t builtins[] = {
		{"exit", exit_and_free},
		{NULL, NULL}
	};
	size_t i;

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

/**
 * init_state - function to initialize the state of shell variables
 * upon initial startup
 * @state: struct containing shell state variables
 * @argv: pointer to initially passed command line arguments
 * @envp: pointer to initially passed environment variables
 */
void init_state(struct hsh_state *state, char **argv, char **envp)
{
	state->argv = argv;
	state->env = copyenv(state, envp);
	state->lineptr = NULL;
	state->command = NULL;
	state->vars = NULL;
	state->aliases = NULL;
	state->linesize = 0;
	state->commandcount = 0;
	state->envsize = countenv(state->env);
	set_builtins(state);
}

/**
 * exit_wrap - wrapper to exit_and_free for use as a builtin to
 * ensure that arguments to `exit' are of numeric type.
 * @state: struct containing shell state variables
 */
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

/**
 * exit_and_free - function to free all mallocated memory and exit
 * gracefully following an error or call to `exit [...]'
 * @state: struct containing shell state variables
 */
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
	free(builtins);
}

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
